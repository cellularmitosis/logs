#include "crc.h"
#include "Si7021.h"

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
  float ambient_c = ambient.getTemp();
  float ambient_rh = ambient.getRH();

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
  
  delay(1000);
}

