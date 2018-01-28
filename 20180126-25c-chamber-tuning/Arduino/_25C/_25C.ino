#include "PID_v1.h"
#include "Si7021.h"

// --- user-configurable variables ---

#define AMBIENT_SCL_PIN 11
#define AMBIENT_SDA_PIN 12

double kp = 10;
double ki = 0.1;
double kd = 0;

// note: the scale is about 88 ADC counts per degree C.
// 29C corresponds to ADC value 759
// 25C corresponds to ADC value 407
// 23C corresponds to ADC value 231
double setpoint = 406;

uint16_t loop_period = 10000; // in ms

// my external Si7021 reads higher than the internal Si7021
float ambient_correction = -0.086;

// ---

double input;
double output;
PID myPID(&input, &output, &setpoint, kp, ki, kd, DIRECT);

uint32_t next_loop_start = 0;

Weather internal;

SoftwareWire ambientWire(AMBIENT_SDA_PIN, AMBIENT_SCL_PIN);
Weather ambient(&ambientWire);

void setup() {
  // Tie the 3.3V pin to the AREF pin for a slightly cleaner AREF.
  // Thanks to https://learn.adafruit.com/thermistor/using-a-thermistor
  analogReference(EXTERNAL);

  pinMode(6, OUTPUT);
  pinMode(A0, INPUT);
  Serial.begin(9600); // 9600 8N1

  ambient.begin();
  internal.begin();

  myPID.SetSampleTime(loop_period);
  myPID.SetMode(AUTOMATIC);

  // print the csv header
  delay(250);
  Serial.println("si_7021_ambient_c,si7021_internal_c,setpoint,adc,adc_c,error_c,output,io_delta_c");

  next_loop_start = millis() + loop_period;
}

double adc_to_degrees_c(double adc) {
  double c_origin = 25.0;
  double lsb_offset = 407.0;
  double lsb_per_c = 88.0;
  return c_origin + ((adc - lsb_offset) / lsb_per_c);
}

void loop() {

  float ambient_c = ambient.getTemp() + ambient_correction;
  float internal_c = internal.getTemp();

  // accumulate samples until we only have 50ms of time margin left in this loop
  uint32_t accumulator = 0;
  uint32_t samples = 0;
  while (next_loop_start - millis() > 50) {
    for (uint8_t i = 0; i < UINT8_MAX; i++) {
      accumulator += analogRead(A0);
    }
    samples += UINT8_MAX;
  }
  input = double(accumulator) / double(samples);

  while (myPID.Compute() == false) { continue; }
  analogWrite(6, output);

//  Serial.println(next_loop_start - millis()); // print out how much time margin was left
  while (millis() <= next_loop_start) { delay(1); } // consume the rest of the time left in this loop
  while (millis() > next_loop_start) { next_loop_start += loop_period; } // bump next_loop_start

  Serial.print(ambient_c, 3);
  Serial.print(",");
  Serial.print(internal_c, 3);
  Serial.print(",");
  Serial.print(setpoint, 0);
  Serial.print(",");
  Serial.print(input, 3);
  Serial.print(",");
  float adc_c = adc_to_degrees_c(input);
  Serial.print(adc_c, 4);
  Serial.print(",");
  float setpoint_c = adc_to_degrees_c(setpoint);
  float error_c = adc_c - setpoint_c;
  Serial.print(error_c, 4);
  Serial.print(",");
  Serial.print(output, 2);
  Serial.print(",");
  float delta_c = adc_c - ambient_c;
  Serial.println(delta_c, 4);
}

