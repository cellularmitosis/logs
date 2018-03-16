#!/usr/bin/env python

import sys

def crc16_xmodem(data):
    # Returns the hex digits of the CRC16 CCITT (XModem) value.
    # Thanks to https://stackoverflow.com/a/30357446
    crc = 0
    msb = crc >> 8
    lsb = crc & 0xFF
    for c in data:
        x = ord(c) ^ msb
        x ^= (x >> 4)
        msb = (lsb ^ (x >> 3) ^ (x << 4)) & 0xFF
        lsb = (x ^ (x << 5)) & 0xFF
    crc = (msb << 8) + lsb
    return hex(crc)[2:]

assert crc16_xmodem("hello") == "c362"
assert crc16_xmodem("123456789") == "31c3"

for line in sys.stdin.readlines():
    line = line.rstrip()
    (data, crc) = line.rsplit(',', 1)
    print "calculated crc of '%s': %s" % (data, crc16_xmodem(data))
