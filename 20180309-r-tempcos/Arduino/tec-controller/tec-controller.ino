#include "buffer.h"
#include "PID_v1.h"
#include "Si7021.h"
#include "AnyWire.h"


// --- Tweakable values:

#define INPUT_PIN A0
#define OUTPUT_PIN 9
#define LED_PIN 13

// reasonable values for 5V with limit of 205:
double heat_kp = 16.0;
double heat_ki = 0.25;
double heat_kd = 0.0;

//double upper_limit = 65.0; // approximately 1000mA at 12V.
double upper_limit = 205.0; // approximately 1000mA at 5V.
double lower_limit = 0.0;

uint16_t loop_period = 1000; // in ms

// thermistor used: Vishay NTCLG100E2103JB
// http://www.vishay.com/docs/29050/ntclg100.pdf
// https://www.digikey.com/products/en?keywords=NTCLG100E2103JB
float thermistor_nom = 11800.0;
float temp_nom = 20.45;
float b_coefficient = 3977;
float series_r = 9853.0;


// --- Global vars

double setpoint = 0;
double input = 0;
double output = 0;
uint16_t accumulator = 0;

uint32_t start = 0;
uint32_t next_loop_start = 0;

bool verbose = true;

Weather ambient; // Si7021 sensor
PID myPID(&input, &output, &setpoint, heat_kp, heat_ki, heat_kd, REVERSE);


// --- Functions

// Adapted from https://learn.adafruit.com/thermistor/using-a-thermistor
float thermistor_adc_to_c(float thermistor) {
  // convert the value to resistance
  float therm_r = series_r / ((1023 / thermistor) - 1);
 
  float steinhart;
  steinhart = therm_r / thermistor_nom;        // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= b_coefficient;                  // 1/B * ln(R/Ro)
  steinhart += 1.0 / (temp_nom + 273.15);      // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert to C
  return steinhart;
}

float thermistor_c_to_adc(float c) {
  float k = c + 273.15;
  float steinhart = 1.0 / k;
  steinhart -= 1.0 / (temp_nom + 273.15);
  steinhart *= b_coefficient;
  steinhart = exp(steinhart);
  float therm_r = steinhart * thermistor_nom;
  float adc = (therm_r / (series_r + therm_r)) * 1023;
  return adc;
}

void toggle_led() {
  if (digitalRead(LED_PIN) > 0) {
    digitalWrite(LED_PIN, LOW);
  } else {
    digitalWrite(LED_PIN, HIGH);    
  }
}


// --- Temperature programs

// test program
float program0[] = {
  25.0,
  26.0,
};

// 1 hour at 25C (the first two segments are silent)
float program1[] = {
  25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0,
};

// 1 ramp (up) of 10C
float program2[] = {
  25.0, 25.0, 25.0, 25.0, 25.0, 25.0,
  26.0, 27.0, 28.0, 29.0, 30.0, 31.0, 32.0, 33.0, 34.0,
  35.0, 35.0, 35.0, 35.0, 35.0, 35.0,
  25.0, 25.0, 25.0, 25.0, 25.0, 25.0,
};

// 1 ramp (up and down) of 10C
float program3[] = {
  25.0, 25.0, 25.0, 25.0, 25.0, 25.0,

  26.0, 27.0, 28.0, 29.0, 30.0, 31.0, 32.0, 33.0, 34.0,
  35.0, 35.0, 35.0, 35.0, 35.0, 35.0,
  34.0, 33.0, 32.0, 31.0, 30.0, 29.0, 28.0, 27.0, 26.0,

  25.0, 25.0, 25.0, 25.0, 25.0, 25.0,
};

// 2 ramps (up and down) of 10C
float program4[] = {
  25.0, 25.0, 25.0, 25.0, 25.0, 25.0,

  26.0, 27.0, 28.0, 29.0, 30.0, 31.0, 32.0, 33.0, 34.0,
  35.0, 35.0, 35.0, 35.0, 35.0, 35.0,
  34.0, 33.0, 32.0, 31.0, 30.0, 29.0, 28.0, 27.0, 26.0,

  25.0, 25.0, 25.0, 25.0, 25.0, 25.0,

  26.0, 27.0, 28.0, 29.0, 30.0, 31.0, 32.0, 33.0, 34.0,
  35.0, 35.0, 35.0, 35.0, 35.0, 35.0,
  34.0, 33.0, 32.0, 31.0, 30.0, 29.0, 28.0, 27.0, 26.0,

  25.0, 25.0, 25.0, 25.0, 25.0, 25.0,
};

// 3 ramps (up and down) of 10C
float program5[] = {
  25.0, 25.0, 25.0, 25.0, 25.0, 25.0,

  26.0, 27.0, 28.0, 29.0, 30.0, 31.0, 32.0, 33.0, 34.0,
  35.0, 35.0, 35.0, 35.0, 35.0, 35.0,
  34.0, 33.0, 32.0, 31.0, 30.0, 29.0, 28.0, 27.0, 26.0,

  25.0, 25.0, 25.0, 25.0, 25.0, 25.0,

  26.0, 27.0, 28.0, 29.0, 30.0, 31.0, 32.0, 33.0, 34.0,
  35.0, 35.0, 35.0, 35.0, 35.0, 35.0,
  34.0, 33.0, 32.0, 31.0, 30.0, 29.0, 28.0, 27.0, 26.0,

  25.0, 25.0, 25.0, 25.0, 25.0, 25.0,

  26.0, 27.0, 28.0, 29.0, 30.0, 31.0, 32.0, 33.0, 34.0,
  35.0, 35.0, 35.0, 35.0, 35.0, 35.0,
  34.0, 33.0, 32.0, 31.0, 30.0, 29.0, 28.0, 27.0, 26.0,

  25.0, 25.0, 25.0, 25.0, 25.0, 25.0,
};

// 7 ramps (up and down) of 10C
float program6[] = {
  25.0, 25.0, 25.0, 25.0, 25.0, 25.0,

  26.0, 27.0, 28.0, 29.0, 30.0, 31.0, 32.0, 33.0, 34.0,
  35.0, 35.0, 35.0, 35.0, 35.0, 35.0,
  34.0, 33.0, 32.0, 31.0, 30.0, 29.0, 28.0, 27.0, 26.0,

  25.0, 25.0, 25.0, 25.0, 25.0, 25.0,

  26.0, 27.0, 28.0, 29.0, 30.0, 31.0, 32.0, 33.0, 34.0,
  35.0, 35.0, 35.0, 35.0, 35.0, 35.0,
  34.0, 33.0, 32.0, 31.0, 30.0, 29.0, 28.0, 27.0, 26.0,

  25.0, 25.0, 25.0, 25.0, 25.0, 25.0,

  26.0, 27.0, 28.0, 29.0, 30.0, 31.0, 32.0, 33.0, 34.0,
  35.0, 35.0, 35.0, 35.0, 35.0, 35.0,
  34.0, 33.0, 32.0, 31.0, 30.0, 29.0, 28.0, 27.0, 26.0,

  25.0, 25.0, 25.0, 25.0, 25.0, 25.0,

  26.0, 27.0, 28.0, 29.0, 30.0, 31.0, 32.0, 33.0, 34.0,
  35.0, 35.0, 35.0, 35.0, 35.0, 35.0,
  34.0, 33.0, 32.0, 31.0, 30.0, 29.0, 28.0, 27.0, 26.0,

  25.0, 25.0, 25.0, 25.0, 25.0, 25.0,

  26.0, 27.0, 28.0, 29.0, 30.0, 31.0, 32.0, 33.0, 34.0,
  35.0, 35.0, 35.0, 35.0, 35.0, 35.0,
  34.0, 33.0, 32.0, 31.0, 30.0, 29.0, 28.0, 27.0, 26.0,

  25.0, 25.0, 25.0, 25.0, 25.0, 25.0,

  26.0, 27.0, 28.0, 29.0, 30.0, 31.0, 32.0, 33.0, 34.0,
  35.0, 35.0, 35.0, 35.0, 35.0, 35.0,
  34.0, 33.0, 32.0, 31.0, 30.0, 29.0, 28.0, 27.0, 26.0,

  25.0, 25.0, 25.0, 25.0, 25.0, 25.0,

  26.0, 27.0, 28.0, 29.0, 30.0, 31.0, 32.0, 33.0, 34.0,
  35.0, 35.0, 35.0, 35.0, 35.0, 35.0,
  34.0, 33.0, 32.0, 31.0, 30.0, 29.0, 28.0, 27.0, 26.0,

  25.0, 25.0, 25.0, 25.0, 25.0, 25.0,
};

// 1 step of 10C (up and down), 30min/plateau.
float program7[] = {
  25.0, 25.0, 25.0, 25.0, 25.0, 25.0,
  35.0, 35.0, 35.0, 35.0, 35.0, 35.0,
  25.0, 25.0, 25.0, 25.0, 25.0, 25.0,
};

uint8_t pwm_bits = 9;

void programmed_setup() {

  ambient.begin();

  // Tie the 3.3V pin to the AREF pin for a slightly cleaner AREF.
  // Thanks to https://learn.adafruit.com/thermistor/using-a-thermistor
  // Update: actually, with an external filter that's too clean.  Using
  // the 5V (noisy) USB line gives better ADC resolution with oversampling.
  analogReference(EXTERNAL);
  
  pinMode(INPUT_PIN, INPUT);
  pinMode(OUTPUT_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  
  Serial.begin(9600); // 9600 8N1

  myPID.SetOutputLimits(lower_limit, upper_limit);
  myPID.SetSampleTime(loop_period);
  myPID.SetMode(AUTOMATIC);

  // print out the CSV header.
  delay(250);
  //Serial.println("set_c,oven_c,output,ambient_c");

  start = millis();
  next_loop_start = millis() + loop_period;
}


void programmed_loop(float *program, uint16_t num_steps, uint32_t step_interval) {
  static int16_t current_step = -1;

  int16_t desired_step = int16_t(floor((millis() - start) / double(step_interval)));

  // end of program.
  if (desired_step == num_steps) {
    analogWrite16(OUTPUT_PIN, pwm_bits, 0);
    Serial.println("debug: end of program.");
    while (true) { delay(1); }
  }

  if (current_step < desired_step) {
    current_step = desired_step;
    float new_setpoint_c = program[current_step];
    setpoint = thermistor_c_to_adc(new_setpoint_c);
    // verbose remains off during the first two programmed steps.
    if (current_step < 2) {
      Serial.println("debug: warm-up at 25C");
      verbose = false;
    } else {
      Serial.print("debug: setpoint = ");      
      Serial.println(new_setpoint_c, 1);
      verbose = true;
    }
  }
  
  float ambient_c = ambient.getTemp();

  // accumulate samples until we only have 50ms of time margin left in this loop
  uint32_t accumulator = 0;
  uint32_t samples = 0;
  while (next_loop_start - millis() > 50) {
    for (uint8_t i = 0; i < UINT8_MAX; i++) {
      accumulator += analogRead(INPUT_PIN);
    }
    samples += UINT8_MAX;
  }
  input = double(accumulator) / double(samples);

  while (myPID.Compute() == false) { continue; }
  analogWrite16(OUTPUT_PIN, pwm_bits, output);
  
//  Serial.println(next_loop_start - millis()); // print out how much time margin was left
  while (millis() <= next_loop_start) { delay(1); }
  while (millis() > next_loop_start) { next_loop_start += loop_period; }

  if (verbose == false) {
    Serial.print("debug: ");
  }
  float set_c = thermistor_adc_to_c(setpoint);
  Serial.print(set_c, 3);
  Serial.print(",");
  float c = thermistor_adc_to_c(input);
  Serial.print(c, 3);
  Serial.print(",");
  Serial.print(output);
  Serial.print(",");
  Serial.println(ambient_c, 3);
  Serial.flush();
  
  toggle_led();
}

// Thanks to https://arduino.stackexchange.com/q/12718
void setupPWM16(uint8_t bits) {
    DDRB |= _BV(PB1) | _BV(PB2);        /* set pins as outputs */
    TCCR1A = _BV(COM1A1) | _BV(COM1B1)  /* non-inverting PWM */
        | _BV(WGM11);                   /* mode 14: fast PWM, TOP=ICR1 */
    TCCR1B = _BV(WGM13) | _BV(WGM12)
        | _BV(CS11);                    /* prescaler 1 */
    ICR1 = (0x0100 << (bits - 8)) - 1;  /* TOP counter value (freeing OCR1A*/
}
/* Comments about the setup
Changing ICR1 will effect the amount of bits of resolution.
ICR1 = 0xffff; (65535) 16-bit resolution
ICR1 = 0x7FFF; (32767) 15-bit resolution
ICR1 = 0x3FFF; (16383) 14-bit resolution etc....

Changing the prescaler will effect the frequency of the PWM signal.
Frequency[Hz}=CPU/(ICR1+1) where in this case CPU=16 MHz
16-bit PWM will be>>> 16000000/(65535+1)=244,14Hz
*/

// Thanks to https://arduino.stackexchange.com/q/12718
/* 16-bit version of analogWrite(). Works only on pins 9 and 10. */
// bits is the number of bits you set this PWM channel to use with setupPWM16().
// val should be a float from 0 to 255.
void analogWrite16(uint8_t pin, uint8_t bits, float val) {
  analogWrite(pin, val);
  
//  uint16_t val16;
//  if (bits > 8) {
//    val16 = val * (0x2 << (bits - 8));
//  } else {
//    val16 = val;
//  }
//  switch (pin) {
//    case  9:
//    OCR1A = val16;
//    break;
//    
//    case 10:
//    OCR1B = val16;
//    break;
//    
//    }
}


void setup() {
//  interactive_setup();
  programmed_setup();
//  setupPWM16(pwm_bits);
}

// programs:
// 0: test program
// 1: 1 hour at 25C (the first two segments are silent) (14 steps)
// 2: 1 ramp (up) of 10C (28 steps)
// 3: 1 ramp (up and down) of 10C (38 steps)
// 4: 2 ramps (up and down) of 10C (54 steps)
// 5: 3 ramps (up and down) of 10C (70 steps)
// 6: 7 ramps (up and down) of 10C
// 7: 1 step of 10C (up and down), 30min plateaus.

void loop () {
  static uint32_t step_interval = 5 * 60 * 1000ul; // 5 minutes per step
  
//  interactive_loop();
//  programmed_loop(program0, sizeof(program0) / sizeof(float), step_interval);
//  programmed_loop(program1, sizeof(program1) / sizeof(float), step_interval);
//  programmed_loop(program2, sizeof(program2) / sizeof(float), step_interval);
//  programmed_loop(program3, sizeof(program3) / sizeof(float), step_interval);
//  programmed_loop(program4, sizeof(program4) / sizeof(float), step_interval);
//  programmed_loop(program5, sizeof(program5) / sizeof(float), step_interval);
//  programmed_loop(program6, sizeof(program6) / sizeof(float), step_interval);
  programmed_loop(program7, sizeof(program7) / sizeof(float), step_interval);
}

