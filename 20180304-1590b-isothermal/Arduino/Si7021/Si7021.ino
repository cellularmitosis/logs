#include <util/crc16.h>

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
}

uint16_t crc16(uint8_t *data, uint16_t len) {
  uint16_t crc = 0;
  for (uint16_t i = 0; i < len; i++) {
    // Note: this is CRC16 CCITT XModem
    crc = _crc_xmodem_update(crc, data[i]);
  }
  return crc;
}

// Appends the hex value of a 16-bit CRC to a string to a CSV row.
// Note: just the four hex characters are appended (e.g. "3c3f"), without
// any leading "0x" notation.
// The appended string is null-terminated, and a pointer which points
// at the terminating null is returned.
char* csv_append_hex_crc16(uint16_t crc, char *buf) {
  // originally inspired by http://stackoverflow.com/a/12839870/558735 
  char map[16+1] = "0123456789abcdef";
  uint8_t high_nibble;
  uint8_t low_nibble;
  uint8_t msb = crc >> 8;
  uint8_t lsb = crc & 0xFF;

  *buf = ','; buf++;

  high_nibble = (msb & 0xF0) >> 4;
  *buf = map[high_nibble]; buf++;

  low_nibble = msb & 0x0F;
  *buf = map[low_nibble]; buf++;

  high_nibble = (lsb & 0xF0) >> 4;
  *buf = map[high_nibble]; buf++;

  low_nibble = lsb & 0x0F;
  *buf = map[low_nibble]; buf++;

  *buf = '\0';
  return buf;
}

void loop() {
  char buf[15]; // e.g. "123456789,ffff\0"
  char *ptr = buf;
  ptr += snprintf(ptr, sizeof(buf), "123456789");
  uint16_t crc = crc16(buf, strlen(buf));
  ptr = csv_append_hex_crc16(crc, ptr);
  Serial.println(buf);
  
  delay(1000);
}

