#include "crc.h"
#include "Si7021.h"
#include <SoftwareSerial.h>

// Average together this many samples.
uint8_t oversample = 1;

Weather regulated; // Si7021 sensor

SoftwareSerial isoser(2,3); // rx, tx
//SoftwareSerial isoser(3,2); // rx, tx

void setup() {
  regulated.begin();
  
  Serial.begin(9600);
  isoser.begin(9600);
  
  Serial.println("STARTING");

  // Print the CSV header.
  Serial.println("isothermal_c,isothermal_rh,crc16");
}

void loop() {
  if (isoser.available()) {
    char ch = isoser.read();
    Serial.print(ch);
  }
}

void loop2() {
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

  while(true) {
    *ptr = isoser.read();
    if (*ptr == '\n') {
      *ptr = '\0';
      break;      
    }
    ptr++;
  }
  Serial.println(buf);

  ptr = buf;

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

