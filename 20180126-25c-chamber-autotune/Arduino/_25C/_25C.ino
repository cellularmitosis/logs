#include "PID_v1.h"

/*
The following Si7021 "Weather" code was adapted from Sparkfun's library.
See https://github.com/sparkfun/Si7021_Breakout/tree/master/Libraries
*/

/*
 SparkFun Si7021 Temperature and Humidity Breakout 
 By: Joel Bartlett
 SparkFun Electronics
 Date: December 10, 2015
 
 This is an Arduino library for the Si7021 Temperature and Humidity Sensor Breakout
 
 This library is based on the following libraries:
 HTU21D Temperature / Humidity Sensor Library
 By: Nathan Seidle
 https://github.com/sparkfun/HTU21D_Breakout/tree/master/Libraries
 Arduino Si7010 relative humidity + temperature sensor
 By: Jakub Kaminski, 2014
 https://github.com/teoqba/ADDRESS
 This Library is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 This Library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 For a copy of the GNU General Public License, see
 <http://www.gnu.org/licenses/>.
 */

#ifndef SparkFun_Si7021_Breakout_Library_h
#define SparkFun_Si7021_Breakout_Library_h

#include <Arduino.h>

/****************Si7021 & HTU21D Definitions***************************/

#define ADDRESS      0x40

#define TEMP_MEASURE_HOLD  0xE3
#define HUMD_MEASURE_HOLD  0xE5
#define TEMP_MEASURE_NOHOLD  0xF3
#define HUMD_MEASURE_NOHOLD  0xF5
#define TEMP_PREV   0xE0

#define WRITE_USER_REG  0xE6
#define READ_USER_REG  0xE7
#define SOFT_RESET  0xFE

#define HTRE        0x02
#define _BV(bit) (1 << (bit))

#define CRC_POLY 0x988000 // Shifted Polynomial for CRC check

// Error codes
#define I2C_TIMEOUT   998
#define BAD_CRC   999


/****************Si7021 & HTU21D Class**************************************/
class Weather
{
public:
  // Constructor
  Weather();

  bool  begin();

  // Si7021 & HTU21D Public Functions
  float getRH();
  float readTemp();
  float getTemp();
  float readTempF();
  float getTempF();
  void  heaterOn();
  void  heaterOff();
  void  changeResolution(uint8_t i);
  void  reset();
  uint8_t  checkID();



private:
  //Si7021 & HTU21D Private Functions
  uint16_t makeMeasurment(uint8_t command);
  void     writeReg(uint8_t value);
  uint8_t  readReg();
};

#endif

/*
 SparkFun Si7021 Temperature and HUmidity Breakout 
 By: Joel Bartlett
 SparkFun Electronics
 Date: December 10, 2015
 
 This is an Arduino library for the Si7021 Temperature and Humidity Sensor Breakout
 
 This library is based on the following libraries:
 HTU21D Temperature / Humidity Sensor Library
 By: Nathan Seidle
 https://github.com/sparkfun/HTU21D_Breakout/tree/master/Libraries
 Arduino Si7010 relative humidity + temperature sensor
 By: Jakub Kaminski, 2014
 https://github.com/teoqba/ADDRESS
 This Library is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 This Library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 For a copy of the GNU General Public License, see
 <http://www.gnu.org/licenses/>.
 */

//#if defined(ARDUINO)
#include "Wire.h"
//#include "SparkFun_Si7021_Breakout_Library.h"
//#elif defined(SPARK)
//#include "SparkFun_Si7021_Breakout_Library/SparkFun_Si7021_Breakout_Library.h"
//#endif


 //Initialize
 Weather::Weather(){}

 bool Weather::begin(void)
{
  Wire.begin();

  uint8_t ID_Temp_Hum = checkID();

  int x = 0;

  if(ID_Temp_Hum == 0x15)//Ping CheckID register
    x = 1;
  else if(ID_Temp_Hum == 0x32)
    x = 2;
  else
    x = 0;

  if(x == 1)
  {
//    Serial.println("Si7021 Found");
    //Serial.println(ID_Temp_Hum, HEX);
  }
  else if(x == 2)
  {
//    Serial.println("HTU21D Found");
    //Serial.println(ID_Temp_Hum, HEX);
  }
  else
  {
//    Serial.println("No Devices Detected");
    //Serial.println(ID_Temp_Hum, HEX);
  }
}

/****************Si7021 & HTU21D Functions**************************************/


float Weather::getRH()
{
  // Measure the relative humidity
  uint16_t RH_Code = makeMeasurment(HUMD_MEASURE_NOHOLD);
  float result = (125.0*RH_Code/65536)-6;
  return result;
}

float Weather::readTemp()
{
  // Read temperature from previous RH measurement.
  uint16_t temp_Code = makeMeasurment(TEMP_PREV);
  float result = (175.72*temp_Code/65536)-46.85;
  return result;
}

float Weather::getTemp()
{
  // Measure temperature
  uint16_t temp_Code = makeMeasurment(TEMP_MEASURE_NOHOLD);
  float result = (175.72*temp_Code/65536)-46.85;
  return result;
}
//Give me temperature in fahrenheit!
float Weather::readTempF()
{
  return((readTemp() * 1.8) + 32.0); // Convert celsius to fahrenheit
}

float Weather::getTempF()
{
  return((getTemp() * 1.8) + 32.0); // Convert celsius to fahrenheit
}


void Weather::heaterOn()
{
  // Turns on the ADDRESS heater
  uint8_t regVal = readReg();
  regVal |= _BV(HTRE);
  //turn on the heater
  writeReg(regVal);
}

void Weather::heaterOff()
{
  // Turns off the ADDRESS heater
  uint8_t regVal = readReg();
  regVal &= ~_BV(HTRE);
  writeReg(regVal);
}

void Weather::changeResolution(uint8_t i)
{
  // Changes to resolution of ADDRESS measurements.
  // Set i to:
  //      RH         Temp
  // 0: 12 bit       14 bit (default)
  // 1:  8 bit       12 bit
  // 2: 10 bit       13 bit
  // 3: 11 bit       11 bit

  uint8_t regVal = readReg();
  // zero resolution bits
  regVal &= 0b011111110;
  switch (i) {
    case 1:
      regVal |= 0b00000001;
      break;
    case 2:
      regVal |= 0b10000000;
      break;
    case 3:
      regVal |= 0b10000001;
    default:
      regVal |= 0b00000000;
      break;
  }
  // write new resolution settings to the register
  writeReg(regVal);
}

void Weather::reset()
{
  //Reset user resister
  writeReg(SOFT_RESET);
}

uint8_t Weather::checkID()
{
  uint8_t ID_1;

  // Check device ID
  Wire.beginTransmission(ADDRESS);
  Wire.write(0xFC);
  Wire.write(0xC9);
  Wire.endTransmission();

    Wire.requestFrom(ADDRESS,1);

    ID_1 = Wire.read();

    return(ID_1);
}

uint16_t Weather::makeMeasurment(uint8_t command)
{
  // Take one ADDRESS measurement given by command.
  // It can be either temperature or relative humidity
  // TODO: implement checksum checking

  uint16_t nBytes = 3;
  // if we are only reading old temperature, read olny msb and lsb
  if (command == 0xE0) nBytes = 2;

  Wire.beginTransmission(ADDRESS);
  Wire.write(command);
  Wire.endTransmission();
  // When not using clock stretching (*_NOHOLD commands) delay here
  // is needed to wait for the measurement.
  // According to datasheet the max. conversion time is ~22ms
   delay(100);
  
  Wire.requestFrom(ADDRESS,nBytes);
  if(Wire.available() != nBytes)
    return 100;

  unsigned int msb = Wire.read();
  unsigned int lsb = Wire.read();
  // Clear the last to bits of LSB to 00.
  // According to datasheet LSB of RH is always xxxxxx10
  lsb &= 0xFC;
  unsigned int mesurment = msb << 8 | lsb;

  return mesurment;
}

void Weather::writeReg(uint8_t value)
{
  // Write to user register on ADDRESS
  Wire.beginTransmission(ADDRESS);
  Wire.write(WRITE_USER_REG);
  Wire.write(value);
  Wire.endTransmission();
}

uint8_t Weather::readReg()
{
  // Read from user register on ADDRESS
  Wire.beginTransmission(ADDRESS);
  Wire.write(READ_USER_REG);
  Wire.endTransmission();
  Wire.requestFrom(ADDRESS,1);
  uint8_t regVal = Wire.read();
  return regVal;
}

// -------------------------

// "main"

Weather si7021;

double kp = 10;
double ki = 0.1;
double kd = 0;

double input;
double output;
double setpoint = 407;

uint32_t next_loop_start = 0;
uint16_t loop_period = 5000; // in ms

PID myPID(&input, &output, &setpoint, kp, ki, kd, DIRECT);

void setup() {
  // Tie the 3.3V pin to the AREF pin for a slightly cleaner AREF.
  // Thanks to https://learn.adafruit.com/thermistor/using-a-thermistor
  analogReference(EXTERNAL);

  pinMode(6, OUTPUT);
  pinMode(A0, INPUT);
  Serial.begin(9600); // 9600 8N1

  si7021.begin();

  myPID.SetMode(AUTOMATIC);

  // print the csv header
  delay(250);
  Serial.println("si7021_c,setpoint,input,output");

  next_loop_start = millis() + loop_period;
}

void loop() {

  float temp_c = si7021.getTemp();

  // accumulate samples until we only have 50ms of margin left in this loop
  uint32_t accumulator = 0;
  uint32_t samples = 0;
  while (next_loop_start - millis() > 50) {
    for (uint8_t i = 0; i < UINT8_MAX; i++) {
      accumulator += analogRead(A0);
    }
    samples += UINT8_MAX;
  }
  input = double(accumulator) / double(samples);    

  myPID.Compute();
  analogWrite(6, output);

//  Serial.println(next_loop_start - millis()); // print out how much time margin was left
  while (millis() <= next_loop_start) { delay(1); } // consume the rest of the time left in this loop
  while (millis() > next_loop_start) { next_loop_start += loop_period; } // bump next_loop_start

  Serial.print(temp_c, 3);
  Serial.print(",");
  Serial.print(setpoint, 0);
  Serial.print(",");
  Serial.print(input, 3);
  Serial.print(",");
  Serial.println(output, 2);
}

