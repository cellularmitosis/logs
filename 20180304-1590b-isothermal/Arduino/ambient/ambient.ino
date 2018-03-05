#include "crc.h"
#include "Si7021.h"

// Average together this many samples.
uint8_t oversample = 16;

Weather ambient; // Si7021 sensor

void setup() {
  ambient.begin();
  Serial.begin(9600);

  // Certain USB-serial chips will buffer output if no receiver is connected.
  // When a receiver then connects, they can get out-of-date data from this buffer.
  // A simple way for the client to clear this is to throw awaty an initial input line(s)
  // if the next line is immediately available.  So, if we add a bit of delay to our first
  // line, the receiver can tell that this line is the start of "fresh" data.
  delay(250);

  // Print the CSV header.
  Serial.println("ambient_c,ambient_rh,crc16");
}

void loop() {
  double rh_accumulator = 0;
  double c_accumulator = 0;
  for(uint8_t i=0; i < oversample; i++) {
    rh_accumulator += ambient.getRH();
    c_accumulator += ambient.readTemp();
  }
  float ambient_rh = rh_accumulator / oversample; 
  float ambient_c = c_accumulator / oversample; 

  char buf[32];
  char *ptr = buf;

  char c_buf[16];
  dtostrf(ambient_c, 1, 3, c_buf);
  
  char rh_buf[16];
  dtostrf(ambient_rh, 1, 3, rh_buf);

  ptr += sprintf(ptr, "%s", c_buf);
  ptr += sprintf(ptr, ",%s", rh_buf);
  uint16_t crc = crc16(buf, strlen(buf));
  ptr = csv_append_hex_crc16(crc, ptr);
  Serial.println(buf);
}

