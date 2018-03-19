#include "crc.h"
#include "Si7021.h"

// --- Configurable parameters ---

// Average together this many samples from the Si7021
uint8_t oversample = 1;

// PID constants
double kp = 16.0;
double ki = 0.25;
double kd = 0.0;
uint16_t loop_period = 1000; // in ms
float set_point_c = 29.0; // in celsius

// exhaust fan
int8_t fanPin = 4;

// --- Global variables ---

// the Si7021 sensor
Weather ambient;
double humidity;
double temperature_c;

double pid_input;
double pid_output = 0;
double pid_setpoint;
PID myPID(&pid_input, &pid_output, &pid_setpoint, kp, ki, kd, DIRECT);

uint32_t start = 0;
uint32_t next_loop_start = 0;

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

void setup() {
  pinMode(fanPin, OUTPUT);

  // start the Si7021
  ambient.begin();

  setpoint_pid = thermistor_c_to_adc(setpoint_c);
  myPID.SetSampleTime(loop_period);
  myPID.SetMode(AUTOMATIC);

  Serial.begin(9600); // 9600 8N1

  // indicate to the client that we are starting.  this allows them to flush old data out of the usb-serial pipe.
  delay(250);
  Serial.println("STARTING");
  
  // Print the CSV header.
  Serial.println("ambient_c,ambient_rh,duty_cycle,crc16");

  start = millis();
  next_loop_start = millis() + loop_period;
}


void loop() {
  
  // toggle the LED to give a visual indication of the control loop activity
  toggle_led();

  // calculate our duty cycle
  float duty_cycle = pid_output / 255.0;
  uint16_t on_time = loop_period * duty_cycle; // in milliseconds
  uint16_t off_time; loop_period - on_time; // in milliseconds

  // turn the fan off
  digitalWrite(fanPin, LOW);

  while(millis() < next_loop_start + off_time) {
    delay(1);
  }

  // turn the fan on
  digitalWrite(fanPin, HIGH);

  // sample the temperature
  if (oversample > 1) { 
    double rh_accumulator = 0;
    double c_accumulator = 0;
    for(uint8_t i=0; i < oversample; i++) {
      rh_accumulator += ambient.getRH();
      c_accumulator += ambient.readTemp();
    }
    humidity = rh_accumulator / oversample; 
    temperature_c = c_accumulator / oversample; 
  } else {
      humidity = ambient.getRH();
      temperature_c = ambient.readTemp();    
  }
  pid_input = temperature_c;

  // construct the logging message
  char buf[32];
  char *ptr = buf;
  char float_buf[16];
  dtostrf(temperature_c, 1, 3, float_buf);
  ptr += sprintf(ptr, "%s", float_buf);
  dtostrf(humidity, 1, 3, rh_buf);
  ptr += sprintf(ptr, ",%s", float_buf);
  dtostrf(duty_cycle, 1, 3, rh_buf);
  ptr += sprintf(ptr, ",%s", float_buf);
  uint16_t crc = crc16(buf, strlen(buf));
  ptr = csv_append_hex_crc16(crc, ptr);

  // print the logging message
  Serial.println(buf);

  // print out how much time margin was left (uncomment this while debugging)
//  Serial.println(next_loop_start - millis());

  // busy-wait until our loop_period is over and then calculate our PID result
  while (myPID.Compute() == false) { continue; }

  // advance the loop timer
  while (millis() <= next_loop_start) { delay(1); }
  while (millis() > next_loop_start) { next_loop_start += loop_period; }
}

