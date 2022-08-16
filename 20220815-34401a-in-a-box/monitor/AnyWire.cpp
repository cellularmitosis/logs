// AnyWire.cpp: abstraction layer for supporting both Wire and SoftwareWire.
// See https://github.com/pepaslabs/AnyWire.

// Copyright 2022 Jason Pepas.
// Released under the terms of the MIT License.
// See https://opensource.org/licenses/MIT.

#include "AnyWire.h"
#include <Wire.h>

AnyWire::AnyWire(SoftwareWire *wire) {
  _softwareWire = wire;
}

AnyWire::~AnyWire() {
}

void AnyWire::begin() {
  if (_softwareWire) {
    _softwareWire->begin();
  } else {
    Wire.begin();
  }
}

void AnyWire::begin(uint8_t addr) {
  if (_softwareWire) {
    _softwareWire->begin(addr);
    while (1) { __asm volatile(""); } // not supported
  } else {
    Wire.begin(addr);
  }
}

void AnyWire::begin(int addr) {
  if (_softwareWire) {
    _softwareWire->begin(addr);
    while (1) { __asm volatile(""); } // not supported
  } else {
    Wire.begin(addr);
  }
}

void AnyWire::end() {
  if (_softwareWire) {
    _softwareWire->end();
  } else {
    Wire.end();
  }
}

void AnyWire::setClock(uint32_t clock) {
  if (_softwareWire) {
    _softwareWire->setClock(clock);
  } else {
    Wire.setClock(clock);
  }
}

void AnyWire::setWireTimeout(uint32_t timeout = 25000, bool reset_with_timeout = false) {
  if (_softwareWire) {
    if (reset_with_timeout) {
      while (1) { __asm volatile(""); } // not supported
    } else {
      _softwareWire->setTimeout(timeout);
    }
  } else {
    Wire.setWireTimeout(timeout, reset_with_timeout);
  }
}

bool AnyWire::getWireTimeoutFlag(void) {
  if (_softwareWire) {
    while (1) { __asm volatile(""); } // not supported
  } else {
    return Wire.getWireTimeoutFlag();
  }
}

void AnyWire::clearWireTimeoutFlag(void) {
  if (_softwareWire) {
    while (1) { __asm volatile(""); } // not supported
  } else {
    Wire.clearWireTimeoutFlag();
  }
}

void AnyWire::beginTransmission(uint8_t address) {
  if (_softwareWire) {
    _softwareWire->beginTransmission(address);
  } else {
    Wire.beginTransmission(address);
  }
}

void AnyWire::beginTransmission(int address) {
  if (_softwareWire) {
    _softwareWire->beginTransmission(address);
  } else {
    Wire.beginTransmission(address);
  }
}

uint8_t AnyWire::endTransmission(void) {
  if (_softwareWire) {
    return _softwareWire->endTransmission();
  } else {
    return Wire.endTransmission();
  }
}

uint8_t AnyWire::endTransmission(uint8_t sendStop) {
  if (_softwareWire) {
    return _softwareWire->endTransmission(sendStop);
  } else {
    return Wire.endTransmission(sendStop);
  }
}

uint8_t AnyWire::requestFrom(uint8_t address, uint8_t quantity) {
  if (_softwareWire) {
    return _softwareWire->requestFrom(address, quantity);
  } else {
    return Wire.requestFrom(address, quantity);
  }
}

uint8_t AnyWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop) {
  if (_softwareWire) {
    return _softwareWire->requestFrom(address, quantity, sendStop);
  } else {
    return Wire.requestFrom(address, quantity, sendStop);
  }
}

uint8_t AnyWire::requestFrom(uint8_t address, uint8_t quantity, uint32_t iaddress, uint8_t isize, uint8_t sendStop) {
  if (_softwareWire) {
    while (1) { __asm volatile(""); } // not supported
  } else {
    return Wire.requestFrom(address, quantity, iaddress, isize, sendStop);
  }
}

uint8_t AnyWire::requestFrom(int address, int quantity) {
  if (_softwareWire) {
    return _softwareWire->requestFrom(address, quantity);
  } else {
    return Wire.requestFrom(address, quantity);
  }
}

uint8_t AnyWire::requestFrom(int address, int quantity, int sendStop) {
  if (_softwareWire) {
    return _softwareWire->requestFrom(address, quantity, sendStop);
  } else {
    return Wire.requestFrom(address, quantity, sendStop);
  }
}

size_t AnyWire::write(uint8_t data) {
  if (_softwareWire) {
    return _softwareWire->write(data);
  } else {
    return Wire.write(data);
  }
}

size_t AnyWire::write(const uint8_t *data, size_t quantity) {
  if (_softwareWire) {
    return _softwareWire->write(data, quantity);
  } else {
    return Wire.write(data, quantity);
  }
}

int AnyWire::available(void) {
  if (_softwareWire) {
    return _softwareWire->available();
  } else {
    return Wire.available();
  }
}

int read(void);

int AnyWire::read(void) {
  if (_softwareWire) {
    return _softwareWire->read();
  } else {
    return Wire.read();
  }
}

int AnyWire::peek(void) {
  if (_softwareWire) {
    return _softwareWire->peek();
  } else {
    return Wire.peek();
  }
}

void AnyWire::flush(void) {
  if (_softwareWire) {
    while (1) { __asm volatile(""); } // not supported
  } else {
    return Wire.peek();
  }
}
