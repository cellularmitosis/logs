#include "crc.h"
#include "PID_v1.h"

// --- Configurable parameters ---

int8_t fan_pin = 3;
int8_t internal_ntc_pin = A0;
int8_t ambient_ntc_pin = A1;

// PID constants
double kp = 125.0;
double ki = 4.0;
double kd = 0.0;
double setpoint_c = 25.0; // in celsius
uint16_t loop_period = 2000; // in ms

// Some notes on PWM values for this particular fan:

// 127 will start
// 31 will run but not start
// 27 will baaaarely run
// 23 will not run

// 255 is 102mA
// 237 is 100mA
// 212 is 90mA
// 187 is 80mA
// 163 is 70mA
// 139 is 60mA
// 115 is 50mA
//  92 is 40mA
//  67 is 30mA
//  43 is 20mA
//  31 is 15mA

uint8_t lower_pwm_limit = 43;
uint8_t upper_pwm_limit = 237;


// --- Global variables ---

double pid_input;
double pid_output = 0;
uint8_t pwm_output = 0;
PID pid(&pid_input, &pid_output, &setpoint_c, kp, ki, kd, REVERSE);

uint32_t start = 0;
uint32_t next_loop_start = 0;


// --- Functions

void setup() {
  analogReference(EXTERNAL);
    
  pinMode(ambient_ntc_pin, INPUT);
  pinMode(internal_ntc_pin, INPUT);
  pinMode(fan_pin, OUTPUT);
  
  Serial.begin(9600); // 9600 8N1
  while(!Serial) {}
  Serial.print("\n\n\n");

  // kick-start the fan to ensure it is initially spinning.
  // (the PWM may later go low enough that the fan would continue if already spinning, but
  // would not start from a dead-stop).
  analogWrite(fan_pin, 237);
  delay(1000);
    
  // Print the CSV header.
//  Serial.println("temp_c,set_c,err_c,duty_cycle,crc16"); // STOPSHIP

  pid.SetSampleTime(loop_period);
  pid.SetMode(AUTOMATIC);

  start = millis();
  next_loop_start = millis() + loop_period;
}

void loop() {

  // update our PWM output
  pwm_output = map(pid_output, 0, 255, lower_pwm_limit, upper_pwm_limit);
  pwm_output = 237; // STOPSHIP
  analogWrite(fan_pin, pwm_output);
    
  uint16_t samples = 8192;
  uint32_t accumulator;
  float ambient_c;
  float internal_c;
    
  accumulator = 0;
  for(uint16_t i=0; i < samples; i++) {
    accumulator += analogRead(internal_ntc_pin);
  }
  internal_c = accumulator / float(samples);
  internal_c = ((512.0 - internal_c) / 100) + 25.0;

  accumulator = 0;
  for(uint16_t i=0; i < samples; i++) {
    accumulator += analogRead(ambient_ntc_pin);
  }
  ambient_c = accumulator / float(samples);
  ambient_c = ((512.0 - ambient_c) / 100) + 25.0;

  pid_input = internal_c;

  // construct the logging message
  char buf[64];
  char *ptr = buf;
  char float_buf[16];
  
  dtostrf(ambient_c, 1, 6, float_buf);
  ptr += sprintf(ptr, "%s", float_buf);
  
  dtostrf(internal_c, 1, 6, float_buf);
  ptr += sprintf(ptr, ",%s", float_buf);
  
//  dtostrf(setpoint_c, 1, 3, float_buf);
//  ptr += sprintf(ptr, ",%s", float_buf);
  
//  dtostrf(temp_c - setpoint_c, 1, 3, float_buf);
//  ptr += sprintf(ptr, ",%s", float_buf);
  
//  dtostrf(pid_output / 2.55, 1, 2, float_buf);
//  ptr += sprintf(ptr, ",%s", float_buf);
    
//  uint16_t crc = crc16(buf, strlen(buf));
//  ptr = csv_append_hex_crc16(crc, ptr);

  // print the logging message
  Serial.println(buf);

  // print out how much time margin was left (uncomment this while debugging)
//  Serial.println(next_loop_start - millis());

  // busy-wait until our loop_period is over and then calculate our PID result
  while (pid.Compute() == false) { continue; }

  // advance the loop timer
  while (millis() <= next_loop_start) { delay(1); }
  while (next_loop_start < millis()) { next_loop_start += loop_period; }
}

