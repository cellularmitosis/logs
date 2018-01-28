/*
The following Si7021 "Weather" code was adapted from Sparkfun's library.
See https://github.com/sparkfun/Si7021_Breakout/tree/master/Libraries
*/

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

#include "Wire.h"
#include "Si7021.h"


//Initialize
Weather::Weather() {
    _wire = AnyWire();
}

Weather::Weather(SoftwareWire *wire) {
    _wire = AnyWire(wire);
}

uint8_t Weather::begin(void) {
  _wire.begin();
  return checkID();
}

/**************** Si7021 & HTU21D Functions **************************************/


float Weather::getRH() {
  // Measure the relative humidity
  uint16_t RH_Code = makeMeasurment(HUMD_MEASURE_NOHOLD);
  float result = (125.0 * RH_Code / 65536) - 6;
  return result;
}

float Weather::readTemp() {
  // Read temperature from previous RH measurement.
  uint16_t temp_Code = makeMeasurment(TEMP_PREV);
  float result = (175.72 * temp_Code / 65536) - 46.85;
  return result;
}

float Weather::getTemp() {
  // Measure temperature
  uint16_t temp_Code = makeMeasurment(TEMP_MEASURE_NOHOLD);
  float result = (175.72 * temp_Code / 65536) - 46.85;
  return result;
}

//Give me temperature in fahrenheit!
float Weather::readTempF() {
  return((readTemp() * 1.8) + 32.0); // Convert celsius to fahrenheit
}

float Weather::getTempF() {
  return((getTemp() * 1.8) + 32.0); // Convert celsius to fahrenheit
}


void Weather::heaterOn() {
  // Turns on the ADDRESS heater
  uint8_t regVal = readReg();
  regVal |= _BV(HTRE);
  //turn on the heater
  writeReg(regVal);
}

void Weather::heaterOff() {
  // Turns off the ADDRESS heater
  uint8_t regVal = readReg();
  regVal &= ~_BV(HTRE);
  writeReg(regVal);
}

void Weather::changeResolution(uint8_t i) {
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

void Weather::reset() {
  //Reset user resister
  writeReg(SOFT_RESET);
}

uint8_t Weather::checkID() {
  uint8_t ID_1;

  // Check device ID
  _wire.beginTransmission(ADDRESS);
  _wire.write(0xFC);
  _wire.write(0xC9);
  _wire.endTransmission();

  _wire.requestFrom(ADDRESS,1);

  ID_1 = _wire.read();

  // Si7021 will return 0x15 (defined as SI7021_ID)
  // HTU21D will return 0x32 (defined as HTU21D_ID)
  return ID_1;
}

uint16_t Weather::makeMeasurment(uint8_t command) {
  // Take one ADDRESS measurement given by command.
  // It can be either temperature or relative humidity
  // TODO: implement checksum checking

  uint16_t nBytes = 3;
  // if we are only reading old temperature, read olny msb and lsb
  if (command == 0xE0) {
      nBytes = 2;
  }

  _wire.beginTransmission(ADDRESS);
  _wire.write(command);
  _wire.endTransmission();
  // When not using clock stretching (*_NOHOLD commands) delay here
  // is needed to wait for the measurement.
  // According to datasheet the max. conversion time is ~22ms
  delay(100);
  
  _wire.requestFrom(ADDRESS,nBytes);
  if (_wire.available() != nBytes) {
    return 100;
  }

  unsigned int msb = _wire.read();
  unsigned int lsb = _wire.read();
  // Clear the last to bits of LSB to 00.
  // According to datasheet LSB of RH is always xxxxxx10
  lsb &= 0xFC;
  unsigned int mesurment = msb << 8 | lsb;

  return mesurment;
}

void Weather::writeReg(uint8_t value) {
  // Write to user register on ADDRESS
  _wire.beginTransmission(ADDRESS);
  _wire.write(WRITE_USER_REG);
  _wire.write(value);
  _wire.endTransmission();
}

uint8_t Weather::readReg() {
  // Read from user register on ADDRESS
  _wire.beginTransmission(ADDRESS);
  _wire.write(READ_USER_REG);
  _wire.endTransmission();
  _wire.requestFrom(ADDRESS,1);
  uint8_t regVal = _wire.read();
  return regVal;
}
