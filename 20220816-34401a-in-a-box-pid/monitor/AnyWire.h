// AnyWire.h: abstraction layer for supporting both Wire and SoftwareWire.
// See https://github.com/pepaslabs/AnyWire.

// Copyright 2022 Jason Pepas.
// Released under the terms of the MIT License.
// See https://opensource.org/licenses/MIT.

#ifndef AnyWire_h
#define AnyWire_h

#include <Arduino.h>
#include "SoftwareWire.h"

class AnyWire {
public:
  AnyWire(SoftwareWire *wire = NULL);
  ~AnyWire();

  void begin();
  void begin(uint8_t addr);
  void begin(int addr);
  void end();
  void setClock(uint32_t clock);
  void setWireTimeout(uint32_t timeout = 25000, bool reset_with_timeout = false);
  bool getWireTimeoutFlag(void);
  void clearWireTimeoutFlag(void);
  void beginTransmission(uint8_t address);
  void beginTransmission(int address);
  uint8_t endTransmission(void);
  uint8_t endTransmission(uint8_t sendStop);
  uint8_t requestFrom(uint8_t address, uint8_t quantity);
  uint8_t requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop);
  uint8_t requestFrom(uint8_t address, uint8_t quantity, uint32_t iaddress, uint8_t isize, uint8_t sendStop);
  uint8_t requestFrom(int address, int quantity);
  uint8_t requestFrom(int address, int quantity, int sendStop);
  size_t write(uint8_t data);
  size_t write(const uint8_t *data, size_t quantity);
  int available(void);
  int read(void);
  int peek(void);
  void flush(void);

private:
  SoftwareWire *_softwareWire;
};

#endif // AnyWire_h
