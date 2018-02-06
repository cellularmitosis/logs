#include "buffer.h"
#include "PID_v1.h"
#include "Si7021.h"
#include "AnyWire.h"

Weather ambient; // Si7021 sensor

#define INPUT_PIN A0
#define OUTPUT_PIN 3
#define LED_PIN 13

double cool_kp = 1.0;
double cool_ki = 3.0;
double cool_kd = 0.5;

// Note: use "reverse" controller direction for heating (and reverse TEC leads)
double heat_kp = 4.0;
double heat_ki = 0.0;
double heat_kd = 0.0;

double upper_limit = 65.0; // approximately 1000mA at 12V.
double lower_limit = 0.0;

// Note:
double setpoint = 0;

double input = 0;
double output = 0;
uint16_t accumulator = 0;

uint16_t loop_period = 1000; // in ms

uint32_t time = 0;
uint32_t start = 0;
uint32_t next_loop_start = 0;

bool pid_running = true;
bool verbose = true;

PID myPID(&input, &output, &setpoint, heat_kp, heat_ki, heat_kd, REVERSE);

#define BUFF_LEN 32
char buffer_bytes[BUFF_LEN];
char_buffer_t buffer = { .len = BUFF_LEN, .bytes = buffer_bytes };
char *read_ptr;

void process_command() {
  /* example command input:
   *  p1.23\n   set Kp to 1.23
   *  i1.23\n   set Ki to 1.23
   *  d1.23\n   set Kd to 1.23
   *  t123.4\n  set the setpoint to ADC value 123.4
   *  c30.0\n   set the setpoint to 30.0 degress Celsius
   *  u255.0\n  set the upper output limit to 255.0
   *  l0.0\n    set the lower output limit to 0.0
   *  start\n   start the control loop
   *  stop\n    stop the control loop
   *  f\n       set controller to "forward" (direct) action
   *  r\n       set controller to "reverse" action
   *  V\n       verbose mode on (log the input and output via serial)
   *  v\n       verbose mode off
   *  ?\n       status dump
   */
  char command = buffer.bytes[0];
  switch (command) {
    case 'p':
      if (myPID.GetDirection() == DIRECT) {
        cool_kp = atof(buffer.bytes+1);
        myPID.SetTunings(cool_kp, cool_ki, cool_kd);
        Serial.print("Kp: ");
        Serial.println(cool_kp, 4);
      } else if (myPID.GetDirection() == REVERSE) {
        heat_kp = atof(buffer.bytes+1);
        myPID.SetTunings(heat_kp, heat_ki, heat_kd);
        Serial.print("Kp: ");
        Serial.println(heat_kp, 4);
      }
      break;
    case 'i':
      if (myPID.GetDirection() == DIRECT) {
        cool_ki = atof(buffer.bytes+1);
        myPID.SetTunings(cool_kp, cool_ki, cool_kd);
        Serial.print("Ki: ");
        Serial.println(cool_ki, 4);
      } else if (myPID.GetDirection() == REVERSE) {
        heat_ki = atof(buffer.bytes+1);
        myPID.SetTunings(heat_kp, heat_ki, heat_kd);
        Serial.print("Ki: ");
        Serial.println(heat_ki, 4);
      }
      break;
    case 'd':
      if (myPID.GetDirection() == DIRECT) {
        cool_kd = atof(buffer.bytes+1);
        myPID.SetTunings(cool_kp, cool_ki, cool_kd);
        Serial.print("Kd: ");
        Serial.println(cool_kd, 4);
      } else if (myPID.GetDirection() == REVERSE) {
        heat_kd = atof(buffer.bytes+1);
        myPID.SetTunings(heat_kp, heat_ki, heat_kd);
        Serial.print("Kd: ");
        Serial.println(heat_kd, 4);
      }
      break;
    case 't':
      setpoint = atof(buffer.bytes+1);
      Serial.print("set: ");
      Serial.println(setpoint, 2);
      break;
    case 'c':
      setpoint = thermistor_c_to_adc(atof(buffer.bytes+1));
      Serial.print("set: ");
      Serial.println(setpoint, 2);
      break;
    case 'u':
      upper_limit = atof(buffer.bytes+1);
      myPID.SetOutputLimits(lower_limit, upper_limit);
      Serial.print("upper limit: ");
      Serial.println(upper_limit, 2);
      break;
    case 'l':
      lower_limit = atof(buffer.bytes+1);
      myPID.SetOutputLimits(lower_limit, upper_limit);
      Serial.print("lower_limit: ");
      Serial.println(lower_limit, 2);
      break;
    case 's':
      if (buffer.bytes[2] == 'a') {
        Serial.println("starting");
        start_pid();
      } else if (buffer.bytes[2] == 'o') {
        Serial.println("stopping");
        stop_pid();
      }
      break;
    case 'f':
      myPID.SetControllerDirection(DIRECT);
      myPID.SetTunings(cool_kp, cool_ki, cool_kd);
      Serial.println("forward");
      break;
    case 'r':
      myPID.SetControllerDirection(REVERSE);
      myPID.SetTunings(heat_kp, heat_ki, heat_kd);
      Serial.println("reverse");
      break;
    case 'V':
      verbose = true;
      break;
    case 'v':
      verbose = false;
      break;
    case '?':
      pid_running ? Serial.println("PID running") : Serial.println("PID stopped");
      Serial.print("heat Kp: ");
      Serial.println(heat_kp, 4);
      Serial.print("heat Ki: ");
      Serial.println(heat_ki, 4);
      Serial.print("heat Kd: ");
      Serial.println(heat_kd, 4);
      Serial.print("cool Kp: ");
      Serial.println(cool_kp, 4);
      Serial.print("cool Ki: ");
      Serial.println(cool_ki, 4);
      Serial.print("cool Kd: ");
      Serial.println(cool_kd, 4);
      Serial.print("set: ");
      Serial.print(setpoint, 2);
      Serial.print(" (");
      Serial.print(thermistor_adc_to_c(setpoint));
      Serial.println("C)");
      Serial.print("upper limit: ");
      Serial.println(upper_limit, 2);
      Serial.print("lower_limit: ");
      Serial.println(lower_limit, 2);
      if (myPID.GetDirection() == DIRECT) {
        Serial.println("forward");
      } else if (myPID.GetDirection() == REVERSE) {
        Serial.println("reverse");
      }
      Serial.print("input: ");
      Serial.println(input, 2);
      Serial.print("output: ");
      Serial.println(output, 2);
      Serial.flush();
      break;
  }

  // Reset the input buffer.
  clear_char_buffer(&buffer);
  read_ptr = buffer.bytes;
}

void start_pid() {
  pid_running = true;
}

void stop_pid() {
  pid_running = false;  
}

// thermistor used: Vishay NTCLG100E2103JB
// http://www.vishay.com/docs/29050/ntclg100.pdf
// https://www.digikey.com/products/en?keywords=NTCLG100E2103JB
float thermistor_nom = 11800.0;
float temp_nom = 20.45;
float b_coefficient = 3977;
float series_r = 9853.0;

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

void interactive_setup() {

  // Tie the 3.3V pin to the AREF pin for a slightly cleaner AREF.
  // Thanks to https://learn.adafruit.com/thermistor/using-a-thermistor
  // Update: actually, with my additional filtering, this ended up being too clean,
  // causing "stair steps" despite oversampling.  Hook it up to the (noisy) 5V instead.
  analogReference(EXTERNAL);
  
  pinMode(INPUT_PIN, INPUT);
  pinMode(OUTPUT_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  
  Serial.begin(9600); // 9600 8N1
  clear_char_buffer(&buffer);
  read_ptr = buffer.bytes;

  myPID.SetSampleTime(loop_period);
  myPID.SetMode(AUTOMATIC);

  time = millis();

  Serial.println("tec-controller");
}

void interactive_loop() {

  // If we have serial input waiting, deal with it.
  if (Serial.available() > 0) {
    char ch = Serial.read();
    if (ch == '\r' || ch == '\n') {
      // If this was the end of a serial command, terminate the buffer and process the command.
      *read_ptr = '\0';
      process_command();
      return;
    } else {
      // Otherwise just add it to the input buffer.
      *read_ptr = ch;
      read_ptr++;
      return;
    }
  }

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

  if (pid_running) {
    analogWrite(OUTPUT_PIN, output);
  } else {
    analogWrite(OUTPUT_PIN, 0);
  }

  if (verbose) {
    Serial.print("i: ");
    Serial.print(input);
    Serial.print(" o: ");
    Serial.print(output);
    float c = thermistor_adc_to_c(input);
    Serial.print(" c: ");
    Serial.println(c);
    Serial.flush();
  }

//  Serial.println(millis() - time);
  while (millis() - time < loop_period) {
    delay(1);
  }
  time += loop_period;
  
  toggle_led();
}

// test program
float program0[2] = {
  25.0,
  26.0,
};

// 1 ramp (up) of 10C
float program1[] = {
  25.0,
  25.0,
  25.0,
  25.0,

  26.0,
  27.0,
  28.0,
  29.0,
  30.0,
  31.0,
  32.0,
  33.0,
  34.0,
  
  35.0,
  35.0,

  25.0,
  25.0,
  25.0,
  25.0,
  25.0,
};


// 3 ramps (up and down) of 10C
float program2[] = {
  25.0,
  25.0,
  25.0,
  25.0,

  26.0,
  27.0,
  28.0,
  29.0,
  30.0,
  31.0,
  32.0,
  33.0,
  34.0,
  
  35.0,
  35.0,
  35.0,

  34.0,
  33.0,
  32.0,
  31.0,
  30.0,
  29.0,
  28.0,
  27.0,
  26.0,
  
  25.0,
  25.0,
  25.0,

  26.0,
  27.0,
  28.0,
  29.0,
  30.0,
  31.0,
  32.0,
  33.0,
  34.0,
  
  35.0,
  35.0,
  35.0,

  34.0,
  33.0,
  32.0,
  31.0,
  30.0,
  29.0,
  28.0,
  27.0,
  26.0,
  
  25.0,
  25.0,
  25.0,

  26.0,
  27.0,
  28.0,
  29.0,
  30.0,
  31.0,
  32.0,
  33.0,
  34.0,
  
  35.0,
  35.0,
  35.0,

  34.0,
  33.0,
  32.0,
  31.0,
  30.0,
  29.0,
  28.0,
  27.0,
  26.0,
  
  25.0,
  25.0,
  25.0,
};

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
  Serial.println("set_c,oven_c,output,ambient_c");

  start = millis();
  next_loop_start = millis() + loop_period;
}


void programmed_loop(float *program, uint8_t num_steps) {
  static uint32_t step_interval = 1 * 60 * 1000ul; // 3 minutes per 1C step
  static int8_t current_step = -1;

  int8_t desired_step = int8_t(floor((millis() - start) / double(step_interval)));

  // end of program.
  if (desired_step == num_steps) {
    analogWrite(OUTPUT_PIN, 0);
    Serial.println("debug: end of program.");
    while (true) { delay(1); }
  }

  if (current_step < desired_step) {
    current_step = desired_step;
    float new_setpoint_c = program[current_step];
    setpoint = thermistor_c_to_adc(new_setpoint_c);
    // verbose remains off during the first two programmed steps.
//    if (current_step < 2) {
    if (false) {
      Serial.println("debug: warm-up at 25C");
//      verbose = false;
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

  if (pid_running) {
    while (myPID.Compute() == false) { continue; }
    analogWrite(OUTPUT_PIN, output);
  } else {
    analogWrite(OUTPUT_PIN, 0);
  }

  
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

void setup() {
//  interactive_setup();
  programmed_setup();
}

void loop () {
//  interactive_loop();
  programmed_loop(program0, sizeof(program0) / sizeof(float));
//  programmed_loop(program1, sizeof(program1) / sizeof(float));
//  programmed_loop(program2, sizeof(program2) / sizeof(float));
}

