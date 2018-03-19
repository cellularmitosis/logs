#ifndef AnyWire_h
#define AnyWire_h

#include <Arduino.h>

class SoftwareWire;

class AnyWire {
public:
  AnyWire();
  AnyWire(SoftwareWire *wire);
  ~AnyWire();

  void begin();
  void begin(uint8_t address);
  void begin(int address);
  void end();
  void setClock(uint32_t clock);
  void beginTransmission(uint8_t address);
  void beginTransmission(int address);
  uint8_t endTransmission(void);
  uint8_t endTransmission(uint8_t sendStop);
  uint8_t requestFrom(uint8_t address, uint8_t quantity);
  uint8_t requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop);
  uint8_t requestFrom(int address, int quantity);
  uint8_t requestFrom(int address, int quantity, int sendStop);
  uint8_t write(uint8_t data);
  uint8_t write(const uint8_t *data, uint8_t size);
  int available(void);
  int read(void);
  int peek(void);
  void flush(void);

private:
  SoftwareWire *_softwareWire;
};

#endif // AnyWire_h
