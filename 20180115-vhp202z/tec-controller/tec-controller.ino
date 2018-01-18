#include "buffer.h"
#include "PID_v1.h"

#define INPUT_PIN A0
#define OUTPUT_PIN 3
#define LED_PIN 13

double cool_kp = 1.0;
double cool_ki = 3.0;
double cool_kd = 0.5;

// Note: use "reverse" controller direction for heating (and reverse TEC leads)
double heat_kp = 7.0;
double heat_ki = 0.75;
double heat_kd = 0.75;

double upper_limit = 255.0;
double lower_limit = 0.0;

// Note:
double setpoint;

double input;
double output;
uint16_t accumulator;

uint16_t loop_period = 250; // in ms

uint32_t time;
uint32_t start;
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

void read_sensor() {
  // Take the average of 64 readings
  // (any more than 64 might overflow the accumulator)
  accumulator = 0;
  for (uint8_t i = 0; i < 64; i++) {
    accumulator += analogRead(INPUT_PIN);  
  }
  input = double(accumulator) / 64;
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

void default_setup() {

  // Tie the 3.3V pin to the AREF pin for a slightly cleaner AREF.
  // Thanks to https://learn.adafruit.com/thermistor/using-a-thermistor
  analogReference(EXTERNAL);
  
  pinMode(INPUT_PIN, INPUT);
  pinMode(OUTPUT_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  
  Serial.begin(9600); // 9600 8N1
  clear_char_buffer(&buffer);
  read_ptr = buffer.bytes;

  //initialize the variables we're linked to
  setpoint = 512;
  read_sensor();

  myPID.SetMode(AUTOMATIC);

  time = millis();

  Serial.println("tec-controller");
}

void default_loop() {

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

  read_sensor();
  myPID.Compute();

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

void program1_setup() {

  // Tie the 3.3V pin to the AREF pin for a slightly cleaner AREF.
  // Thanks to https://learn.adafruit.com/thermistor/using-a-thermistor
  // Update: actually, with an external filter that's too clean.  Using
  // the 5V (noisy) USB line gives better ADC resolution with oversampling.
  analogReference(EXTERNAL);
  
  pinMode(INPUT_PIN, INPUT);
  pinMode(OUTPUT_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  
  Serial.begin(9600); // 9600 8N1
  clear_char_buffer(&buffer);
  read_ptr = buffer.bytes;

  //initialize the variables we're linked to
  setpoint = 512;
  read_sensor();

  myPID.SetMode(AUTOMATIC);

  start = millis();

  verbose = false;
}

int program_step = 0;

void program1_set_c() {
  uint32_t interval = 300000; // 5 minutes per 1C step
  uint32_t now = millis();
  if ((now - start) >= (18 * interval) && program_step < 18) {
    verbose = false;
    Serial.println("debug: verbose off");
    program_step = 18;
    return;
  }
  if ((now - start) >= (13 * interval) && program_step < 13) {
    setpoint = thermistor_c_to_adc(25.0);
    Serial.println("debug: 25C");
    program_step = 13;
    return;
  }
  if ((now - start) >= (12 * interval) && program_step < 12) {
    setpoint = thermistor_c_to_adc(35.0);
    Serial.println("debug: 35C");
    program_step = 12;
    return;
  }
  if ((now - start) >= (11 * interval) && program_step < 11) {
    setpoint = thermistor_c_to_adc(34.0);
    Serial.println("debug: 34C");
    program_step = 11;
    return;
  }
  if ((now - start) >= (10 * interval) && program_step < 10) {
    setpoint = thermistor_c_to_adc(33.0);
    Serial.println("debug: 33C");
    program_step = 10;
    return;
  }
  if ((now - start) >= (9 * interval) && program_step < 9) {
    setpoint = thermistor_c_to_adc(32.0);
    Serial.println("debug: 32C");
    program_step = 9;
    return;
  }
  if ((now - start) >= (8 * interval) && program_step < 8) {
    setpoint = thermistor_c_to_adc(31.0);
    Serial.println("debug: 31C");
    program_step = 8;
    return;
  }
  if ((now - start) >= (7 * interval) && program_step < 7) {
    setpoint = thermistor_c_to_adc(30.0);
    Serial.println("debug: 30C");
    program_step = 7;
    return;
  }
  if ((now - start) >= (6 * interval) && program_step < 6) {
    setpoint = thermistor_c_to_adc(29.0);
    Serial.println("debug: 29C");
    program_step = 6;
    return;
  }
  if ((now - start) >= (5 * interval) && program_step < 5) {
    setpoint = thermistor_c_to_adc(28.0);
    Serial.println("debug: 28C");
    program_step = 5;
    return;
  }
  if ((now - start) >= (4 * interval) && program_step < 4) {
    setpoint = thermistor_c_to_adc(27.0);
    Serial.println("debug: 27C");
    program_step = 4;
    return;
  }
  if ((now - start) >= (3 * interval) && program_step < 3) {
    setpoint = thermistor_c_to_adc(26.0);
    Serial.println("debug: 26C");
    program_step = 3;
    return;
  }
  if ((now - start) >= (2 * interval) && program_step < 2) {
    verbose = true;
    Serial.println("debug: verbose on");
    program_step = 2;
    return;
  }
  if (program_step < 1) {
    Serial.println("debug: starting at 25C");
    setpoint = thermistor_c_to_adc(25.0);
    program_step = 1;
    return;
  }
}

void program1_loop() {
  program1_set_c();

  read_sensor();

  if (pid_running) {
    myPID.Compute();
    analogWrite(OUTPUT_PIN, output);
  } else {
    analogWrite(OUTPUT_PIN, 0);
  }

  if (verbose) {
    float set_c = thermistor_adc_to_c(setpoint);
    Serial.print(set_c, 3);
    Serial.print(", ");
    float c = thermistor_adc_to_c(input);
    Serial.print(c, 3);
    Serial.print(", ");
    Serial.println(output);
    Serial.flush();
  }

  while (millis() > next_loop_start) { next_loop_start += loop_period; }
  while (millis() < next_loop_start) { delay(1); }
  
  toggle_led();
}

void setup() {
//  default_setup();
  program1_setup();
}

void loop () {
//  default_loop();
  program1_loop();
}

