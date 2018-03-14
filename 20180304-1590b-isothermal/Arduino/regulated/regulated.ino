#include "crc.h"
#include "Si7021.h"
#include <SoftwareSerial.h>

// Average together this many samples.
uint8_t oversample = 1;

Weather regulated; // Si7021 sensor

SoftwareSerial isoser(2,3); // rx, tx

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

