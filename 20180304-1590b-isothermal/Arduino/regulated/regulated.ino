#include "crc.h"
#include "Si7021.h"

// Average together this many samples.
uint8_t oversample = 3;

Weather regulated; // Si7021 sensor

void setup() {
  regulated.begin();
  Serial.begin(9600);

  Serial.println("STARTING");

  // Print the CSV header.
  Serial.println("isothermal_c,isothermal_rh,crc16");
}

void loop() {
  double rh_accumulator = 0;
  double c_accumulator = 0;
  for(uint8_t i=0; i < oversample; i++) {
    rh_accumulator += regulated.getRH();
    c_accumulator += regulated.readTemp();
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

