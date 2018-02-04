#include "AnyWire.h"
#include <Arduino.h>
#include <Wire.h>
#include "SoftwareWire.h"

//
// Constructors
//
AnyWire::AnyWire() {
  _softwareWire = NULL;
}

AnyWire::AnyWire(SoftwareWire *wire) {
  _softwareWire = wire;
}

// 
// The destructor releases the pins Software I2C bus for other use.
//
AnyWire::~AnyWire() {
}

// The pins are not changed until begin() is called.
void AnyWire::begin(void) {
  if (_softwareWire) {
    _softwareWire->begin();
  } else {
    Wire.begin();
  }
}

void AnyWire::begin(uint8_t address) {
  if (_softwareWire) {
    _softwareWire->begin(address);
  } else {
    Wire.begin(address);
  }
}

void AnyWire::begin(int address) {
  if (_softwareWire) {
    _softwareWire->begin(address);
  } else {
    Wire.begin(address);
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

uint8_t AnyWire::write(uint8_t data) {
  if (_softwareWire) {
    return _softwareWire->write(data);
  } else {
    return Wire.write(data);
  }
}

uint8_t AnyWire::write(const uint8_t *data, uint8_t size) {
  if (_softwareWire) {
    return _softwareWire->write(data, size);
  } else {
    return Wire.write(data, size);
  }
}

int AnyWire::available(void) {
  if (_softwareWire) {
    return _softwareWire->available();
  } else {
    return Wire.available();
  }
}

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
    return;
  } else {
    Wire.flush();
  }
}

// //
// uint8_t SoftwareWire::write(const uint8_t* data, uint8_t quantity)
// {
//   for (uint8_t i=0; i<quantity; i++) 
//   {
//     write(data[i]);
//   }
  
//   return(quantity);          // ignore any errors, return the number of bytes that are written.
// }


// //
// uint8_t SoftwareWire::write(char* data)
// {
//   int n = strlen(data);
//   write((uint8_t*)data, n);
  
//   return(n);           // ignore any errors, return the number of bytes that are written.
// }


// int SoftwareWire::available(void)
// {
//   return(rxBufPut - rxBufGet);
// }


// //
// int SoftwareWire::peek(void)
// {
//   int data;
  
//   if( rxBufPut > rxBufGet)
//   {
//     data = rxBuf[rxBufGet];
//   }
//   else
//   {
//     data = -1;
//   }
  
//   return(data);
// }


// //
// // The read() reads the buffer, not the I2C bus.
// //
// int SoftwareWire::read(void)
// {
//   int data;
  
//   if( rxBufPut > rxBufGet)
//   {
//     data = rxBuf[rxBufGet];
//     rxBufGet++;
//   }
//   else
//   {
//     data = -1;
//   }
  
//   return(data);
// }


// int SoftwareWire::readBytes(uint8_t* buf, uint8_t size)
// {
//   int data;
//   int n;
  
//   for( n=0; n<size; n++)
//   {
//     data = read();
//     if( data == -1)
//       break;
//     else
//       buf[n] = (uint8_t) data;
//   }
  
//   return(n);
// }


// //
// int SoftwareWire::readBytes(char * buf, uint8_t size)
// {
//   return readBytes( (uint8_t *) buf, size);
// }


// //
// int SoftwareWire::readBytes(char * buf, int size)
// {
//   return readBytes( (uint8_t *) buf, (uint8_t) size);
// }


// //
// // Set the clock speed for the I2C bus.
// // Default is 100000 (100kHz).
// // A speed of 1Hz is possible with this software I2C library (but not with the Arduino Wire library).
// // A speed of 200kHz or higher will remove the delay on an Arduino Uno.
// // Without the delay, the functions are free running, using the execution timing of the code.
// //
// void SoftwareWire::setClock(uint32_t clock)
// {
//   // Tested values with an earlier version of this library.
//   //   Value 0 is without delay, the result depends on the microcontroller and the cpu clock.
//   //   0=maxspeed=140kHz (tested on 328P@16MHz)
//   //   1=120kHz
//   //   2=100kHz (default)
//   //   7=50kHz
//   //   47=10kHz
//   //   97=5kHz
//   //   500=1kHz
//   //   5000=100Hz
//   //   16383=minspeed=30Hz  - delayMicroseconds() max value reference arduino
//   // 

//   // The _i2cdelay is an uint16_t
//   _i2cdelay = ( (F_CPU / 32L) / clock );               // The delay in microseconds, '32' is for this code.
//   unsigned int delayByCode = (F_CPU / 5000000L);       // Add some delay for the code, just a guess
  
//   if( _i2cdelay > delayByCode)
//     _i2cdelay -= delayByCode;
//   else
//     _i2cdelay = 0;

// }


// // 
// // Set the timeout in milliseconds.
// // At this moment, it is only used for timeout when the Slave is stretching the clock pulse.
// //
// void SoftwareWire::setTimeout(long timeout)    
// {
//   // 2017, fix issue #6. 
//   // A signed long as parameter to be compatible with Arduino libraries.
//   // A unsigned long internal to avoid compiler warnings.
//   _timeout = (unsigned long) timeout;
// }


// #ifdef ENABLE_PRINTSTATUS
// //
// // printStatus
// // -----------
// // Print information to the Serial port
// // Used during developing and debugging.
// // Call it with the Serial port as parameter:
// //   myWire.printStatus(Serial);
// // This function is not compatible with the Wire library.
// // When this function is not called, it does not use any memory.
// //
// void SoftwareWire::printStatus( HardwareSerial& Ser)
// {
//   Ser.println(F("-------------------"));
//   Ser.println(F("SoftwareWire Status"));
//   Ser.println(F("-------------------"));
//   Ser.print(F("  F_CPU = "));
//   Ser.println(F_CPU);
//   Ser.print(F("  sizeof(SoftwareWire) = "));
//   Ser.println(sizeof(SoftwareWire));
//   Ser.print(F("  _transmission status = "));
//   Ser.println(_transmission);
//   Ser.print(F("  _i2cdelay = "));
//   Ser.print(_i2cdelay);
//   if( _i2cdelay == 0)
//     Ser.print(F(" (free running)"));
//   Ser.println();
//   Ser.print(F("  _pullups = "));
//   Ser.print(_pullups);
//   if( _pullups)
//     Ser.print(F(" (enabled)"));
//   Ser.println();
//   Ser.print(F("  _timeout = "));
//   Ser.print(_timeout);
//   Ser.println(F(" ms"));

//   Ser.print(F("  SOFTWAREWIRE_BUFSIZE = "));
//   Ser.println(SOFTWAREWIRE_BUFSIZE);
//   Ser.print(F("  rxBufPut = "));
//   Ser.println(rxBufPut);
//   Ser.print(F("  rxBufGet = "));
//   Ser.println(rxBufGet);
//   Ser.print(F("  available() = "));
//   Ser.println(available());
//   Ser.print(F("  rxBuf (hex) = "));
//   for(int ii=0; ii<SOFTWAREWIRE_BUFSIZE; ii++)
//   {
//     if(rxBuf[ii] < 16)
//       Ser.print(F("0"));
//     Ser.print(rxBuf[ii],HEX);
//     Ser.print(F(" "));
//   }
//   Ser.println();
  
//   Ser.print(F("  _sdaPin = "));
//   Ser.println(_sdaPin);
//   Ser.print(F("  _sclPin = "));
//   Ser.println(_sclPin);
//   Ser.print(F("  _sdaBitMast = 0x"));
//   Ser.println(_sdaBitMask, HEX);
//   Ser.print(F("  _sclBitMast = 0x"));
//   Ser.println(_sclBitMask, HEX);
//   Ser.print(F("  _sdaPortReg = "));  
//   Ser.println( (uint16_t) _sdaPortReg, HEX);
//   Ser.print(F("  _sclPortReg = "));  
//   Ser.println( (uint16_t) _sclPortReg, HEX);
//   Ser.print(F("  _sdaDirReg = "));  
//   Ser.println( (uint16_t) _sdaDirReg, HEX);
//   Ser.print(F("  _sclDirReg = "));  
//   Ser.println( (uint16_t) _sclDirReg, HEX);
//   Ser.print(F("  _sdaPinReg = "));  
//   Ser.println( (uint16_t) _sdaPinReg, HEX);
//   Ser.print(F("  _sclPinReg = "));  
//   Ser.println( (uint16_t) _sclPinReg, HEX);
  
//   Ser.print(F("  line state sda = "));
//   Ser.println(i2c_sda_read());
//   Ser.print(F("  line state scl = "));
//   Ser.println(i2c_scl_read());
  
// #ifdef ENABLE_I2C_SCANNER
//   // i2c_scanner
//   // Taken from : http://playground.arduino.cc/Main/I2cScanner
//   // At April 2015, it was version 5
//   Ser.println("\n  I2C Scanner");
//   byte error, address;
//   int nDevices;

//   Ser.println("  Scanning...");

//   nDevices = 0;
//   for(address=1; address<127; address++ ) 
//   {
//     // The i2c_scanner uses the return value of
//     // the Write.endTransmisstion to see if
//     // a device did acknowledge to the address.
//     beginTransmission(address);
//     error = endTransmission();

//     if (error == 0)
//     {
//       Ser.print("  I2C device found at address 0x");
//       if (address<16) 
//         Ser.print("0");
//       Ser.print(address,HEX);
//       Ser.println("  !");

//       nDevices++;
//     }
//     else if (error==4) 
//     {
//       Ser.print("  Unknow error at address 0x");
//       if (address<16) 
//         Ser.print("0");
//       Ser.println(address,HEX);
//     }    
//   }
//   if (nDevices == 0)
//     Ser.println("  No I2C devices found\n");
//   else
//     Ser.println("  done\n");
// #endif    
// }
// #endif


// //--------------------------------------------------------------------


// //
// // The i2c_writebit and i2c_readbit could be make "inline", but that
// // didn't increase the speed, and the code size increases.
// //
// // The sda is low after the start condition.
// // Therefor the sda is low for the first bit.
// //
// void SoftwareWire::i2c_writebit(uint8_t c)
// {
//   if(c==0)
//   {
//     i2c_sda_lo();
//   }
//   else
//   {
//     i2c_sda_hi();
//   }
  
//   if (_i2cdelay != 0)               // This delay is not needed, but it makes it safer
//     delayMicroseconds(_i2cdelay);   // This delay is not needed, but it makes it safer
  
//   i2c_scl_hi();                     // clock high: the Slave will read the sda signal
  
//   // Check if clock stretching by the Slave should be detected.
//   if( _stretch)
//   {
//     // If the Slave was strechting the clock pulse, the clock would not go high immediately.
//     // For example if the Slave is an Arduino, that has other interrupts running (for example Serial data).
//     unsigned long prevMillis = millis();
//     while( i2c_scl_read() == 0)
//     {
//       if( millis() - prevMillis >= _timeout)
//         break;
//     };
//   }

//   // After the clock stretching, the clock must be high for the normal duration.
//   // That is why this delay has still to be done.
//   if (_i2cdelay != 0)
//     delayMicroseconds(_i2cdelay);

//   i2c_scl_lo();
  
//   if (_i2cdelay != 0)
//     delayMicroseconds(_i2cdelay);
// }


// //
// uint8_t SoftwareWire::i2c_readbit(void)
// {
//   i2c_sda_hi();            // 'hi' is the same as releasing the line
//   i2c_scl_hi();

//   // Check if clock stretching by the Slave should be detected.
//   if( _stretch)
//   {
//     // Wait until the clock is high, the Slave could keep it low for clock stretching.
//     unsigned long prevMillis = millis();
//     while( i2c_scl_read() == 0)
//     {
//       if( millis() - prevMillis >= _timeout)
//         break;
//     };
//   }

//   // After the clock stretching, this delay has still be done before reading sda.
//   if (_i2cdelay != 0)
//     delayMicroseconds(_i2cdelay);
  
//   uint8_t c = i2c_sda_read();
  
//   i2c_scl_lo();

//   if (_i2cdelay != 0)
//     delayMicroseconds(_i2cdelay);

//   return(c);
// }


// //
// // Initializes the Software I2C.
// //
// // The original i2c_init sets the SDA and SCL high at the same time.
// //
// // The code has been changed, since the first data to the software i2c did fail sometimes.
// // Changed into SCL high first, with a delay.
// // That would send a STOP if the SDA happens to be low.
// // Any Slave that was busy, will detect the STOP.
// //
// // After both lines are high, the delay is changed into 4 times the normal delay.
// // That did reduce the error with the first tranmission.
// // It was tested with Arduino Uno with clock of 100kHz (_i2cdelay=2).
// // 
// void SoftwareWire::i2c_init(void)
// {
//   i2c_scl_hi();

//   if (_i2cdelay != 0)
//     delayMicroseconds(_i2cdelay);

//   i2c_sda_hi();

//   for( uint8_t i=0; i<4; i++)             // 4 times the normal delay, to claim the bus.
//   {
//     if (_i2cdelay != 0)
//       delayMicroseconds(_i2cdelay);
//   }
// }


// //
// // Send a START Condition
// //
// // The SDA and SCL should already be high.
// // 
// // The SDA and SCL will both be low after this function.
// // When writing the address, the Master makes them high.
// // 
// // Return value:
// //   true  : software i2c bus is okay.
// //   false : failed, some kind of hardware bus error.
// //
// boolean SoftwareWire::i2c_start(void)
// {
//   i2c_sda_hi();              // can perhaps be removed some day ? if the rest of the code is okay
//   i2c_scl_hi();              // can perhaps be removed some day ? if the rest of the code is okay

//   if (_i2cdelay != 0)
//     delayMicroseconds(_i2cdelay);
    
//   // Both the sda and scl should be high.
//   // If not, there might be a hardware problem with the i2c bus signal lines.
//   // This check was added to prevent that a shortcut of sda would be seen as a valid ACK
//   // from a i2c Slave.
//   uint8_t sda_status = i2c_sda_read();
//   uint8_t scl_status = i2c_scl_read();
//   if(sda_status == 0 || scl_status == 0)
//   {
//     return(false);
//   }
//   else
//   {
//     i2c_sda_lo();
    
//     if (_i2cdelay != 0)
//       delayMicroseconds(_i2cdelay);
  
//     i2c_scl_lo();
    
//     if (_i2cdelay != 0)
//       delayMicroseconds(_i2cdelay);
//   }
//   return(true);
// }


// //
// // Repeated START instead of a STOP
// // 
// // TODO: check if the repeated start actually works.
// //
// void SoftwareWire::i2c_repstart(void)
// {
//   i2c_sda_hi();
// //  i2c_scl_hi();               // ??????

//   i2c_scl_lo();                         // force SCL low
  
//   if (_i2cdelay != 0)
//     delayMicroseconds(_i2cdelay);

//   i2c_sda_hi();                        // release SDA
  
//   if (_i2cdelay != 0)
//     delayMicroseconds(_i2cdelay);

//   i2c_scl_hi();                        // release SCL
  
//   // Check if clock stretching by the Slave should be detected.
//   if( _stretch)
//   {
//     // If the Slave was strechting the clock pulse, the clock would not go high immediately.
//     // For example if the Slave is an Arduino, that has other interrupts running (for example Serial data).
//     unsigned long prevMillis = millis();
//     while( i2c_scl_read() == 0)
//     {
//       if( millis() - prevMillis >= _timeout)
//         break;
//     };
//   }
  
//   if (_i2cdelay != 0)
//     delayMicroseconds(_i2cdelay);

//   i2c_sda_lo();                        // force SDA low
  
//   if (_i2cdelay != 0)
//     delayMicroseconds(_i2cdelay);
// }


// // Send a STOP Condition
// //
// // The stop was not recognized by every chip.
// // Some code has been added (with comment "ADDED1"),
// // to be sure that the levels are okay with delays in between.
// void SoftwareWire::i2c_stop(void)
// {
//   i2c_scl_lo();         // ADDED1, it should already be low.
//   i2c_sda_lo();
  
//   // ADDED1, wait to be sure that the slave knows that both are low
//   if (_i2cdelay != 0)              // ADDED1
//     delayMicroseconds(_i2cdelay);  // ADDED1
  
//   // For a stop, make SCL high wile SDA is still low
//   i2c_scl_hi();
  
//   // Check if clock stretching by the Slave should be detected.
//   if( _stretch)
//   {
//     // Wait until the clock is high, the Slave could keep it low for clock stretching.
//     // Clock pulse stretching during a stop condition seems odd, but when
//     // the Slave is an Arduino, it might happen.
//     unsigned long prevMillis = millis();
//     while( i2c_scl_read() == 0)
//     {
//       if( millis() - prevMillis >= _timeout)
//         break;
//     };
//   }
   
//   if (_i2cdelay != 0)
//     delayMicroseconds(_i2cdelay);

//   // complete the STOP by setting SDA high
//   i2c_sda_hi();
  
//   // A delay after the STOP for safety.
//   // It is not known how fast the next START will happen.
//   if (_i2cdelay != 0)
//     delayMicroseconds(_i2cdelay);
// }


// //
// // Write a byte to the I2C slave device
// // The returned bit is 0 for ACK and 1 for NACK
// //
// uint8_t SoftwareWire::i2c_write( uint8_t c )
// {
//   for ( uint8_t i=0; i<8; i++) 
//   {
//     i2c_writebit(c & 0x80);           // highest bit first
//     c <<= 1;
//   }

//   return(i2c_readbit());
// }


// //
// // read a byte from the I2C slave device
// //
// uint8_t SoftwareWire::i2c_read(boolean ack)
// {
//   uint8_t res = 0;

//   for(uint8_t i=0; i<8; i++) 
//   {
//     res <<= 1;
//     res |= i2c_readbit();
//   }

//   if(ack)
//   {
//     i2c_writebit(0);
//   }
//   else
//   {
//     i2c_writebit(1);
//   }

//   if(_i2cdelay != 0)
//     delayMicroseconds(_i2cdelay);

//   return(res);
// }

