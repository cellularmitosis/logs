#include <util/crc16.h>

uint16_t crc16(uint8_t *data, uint16_t len);

// Appends the hex value of a 16-bit CRC to a string to a CSV row.
// Note: just the four hex characters are appended (e.g. "3c3f"), without
// any leading "0x" notation.
// The appended string is null-terminated, and a pointer which points
// at the terminating null is returned.
char* csv_append_hex_crc16(uint16_t crc, char *buf);
