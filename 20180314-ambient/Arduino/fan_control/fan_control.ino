#include "crc.h"
#include "Si7021.h"
#include "PID_v1.h"
//#include <Wire.h>

// --- Configurable parameters ---

// PID constants
double kp = 1000.0;
double ki = 0.0;
double kd = 0.0;
double setpoint_c = 25; // in celsius

// how long it typically takes to read from the Si7012, in milliseconds;
uint8_t temperature_read_delay = 202 + 2; // actually 201 or 202, but we add a small safety margin.
uint16_t loop_period = temperature_read_delay * (9 + 1); // in ms

// exhaust fan
// this is connected to the base of a 2N7000 via a 150k / 1uF low-pass filter
int8_t fanPin = 3;

int8_t ledPin = 13;

// --- Global variables ---

// the Si7021 sensor (outside the box)
uint8_t out_sda_pin = 6;
uint8_t out_scl_pin = 5;
SoftwareWire swire(out_sda_pin, out_scl_pin);
Weather outside(&swire);
float out_humidity;
float out_temperature_c;

// the Si7021 sensor (inside the box)
Weather inside;
double in_humidity;
double in_temperature_c;

double pid_input;
double pid_output = 0;
uint8_t pwm_output = 0;
PID myPID(&pid_input, &pid_output, &setpoint_c, kp, ki, kd, REVERSE);

uint32_t start = 0;
uint32_t next_loop_start = 0;

// --- Functions

void toggle_led() {
  if (digitalRead(ledPin) > 0) {
    digitalWrite(ledPin, LOW);
  } else {
    digitalWrite(ledPin, HIGH);    
  }
}

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(fanPin, OUTPUT);

  Serial.begin(9600); // 9600 8N1
  while(!Serial) {}

  // start the temperature sensors

  while(!outside.begin()) {
    Serial.println("Could not find BME280 sensor!");
    delay(1000);
  }

  while(!inside.begin()) {
    Serial.println("Could not find Si7021 sensor!");
    delay(1000);
  }

  myPID.SetSampleTime(loop_period);
  myPID.SetMode(AUTOMATIC);

  // indicate to the client that we are starting.  this allows them to flush old data out of the usb-serial pipe.
  delay(250);
  Serial.println("\nSTARTING");
  
  // Print the CSV header.
  Serial.println("inside_c,inside_rh,outside_c,outside_rh,set_c,err_c,duty_cycle,crc16");

  start = millis();
  next_loop_start = millis() + loop_period;
}

void loop() {

  // update our PWM output
  pwm_output = map(pid_output, 0, 255, 90, 110);
  analogWrite(fanPin, pwm_output);
  
  // toggle the LED to give a visual indication of the control loop activity
  toggle_led();
  
  double rh_accumulator = 0;
  double c_accumulator = 0;

  rh_accumulator += inside.getRH();
  c_accumulator += inside.readTemp();
  rh_accumulator += inside.getRH();
  c_accumulator += inside.readTemp();
  rh_accumulator += inside.getRH();
  c_accumulator += inside.readTemp();
  rh_accumulator += inside.getRH();
  c_accumulator += inside.readTemp();
  rh_accumulator += inside.getRH();
  c_accumulator += inside.readTemp();
  rh_accumulator += inside.getRH();
  c_accumulator += inside.readTemp();
  rh_accumulator += inside.getRH();
  c_accumulator += inside.readTemp();
  rh_accumulator += inside.getRH();
  c_accumulator += inside.readTemp();
  rh_accumulator += inside.getRH();
  c_accumulator += inside.readTemp();

  in_humidity = rh_accumulator / 9;
  in_temperature_c = c_accumulator / 9;

  out_humidity = outside.getRH();
  out_temperature_c = outside.readTemp();
    
  pid_input = in_temperature_c;

  // construct the logging message
  char buf[64];
  char *ptr = buf;
  char float_buf[16];
  
  dtostrf(in_temperature_c, 1, 3, float_buf);
  ptr += sprintf(ptr, "%s", float_buf);
  
  dtostrf(in_humidity, 1, 3, float_buf);
  ptr += sprintf(ptr, ",%s", float_buf);

  dtostrf(out_temperature_c, 1, 3, float_buf);
  ptr += sprintf(ptr, ",%s", float_buf);
  
  dtostrf(out_humidity, 1, 3, float_buf);
  ptr += sprintf(ptr, ",%s", float_buf);

  dtostrf(setpoint_c, 1, 3, float_buf);
  ptr += sprintf(ptr, ",%s", float_buf);
  
  dtostrf(in_temperature_c - setpoint_c, 1, 3, float_buf);
  ptr += sprintf(ptr, ",%s", float_buf);
  
  dtostrf(pid_output / 2.55, 1, 2, float_buf);
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
  while (next_loop_start < millis()) { next_loop_start += loop_period; }
}

