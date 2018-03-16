#!/usr/bin/env python

import sys
import serial
import time

def crc16_xmodem(data):
    # Returns an integer CRC16 CCITT (XModem) value.
    # Thanks to https://stackoverflow.com/a/30357446
    # Note: use the hex() function to convert this int into a hex string.
    crc = 0
    msb = crc >> 8
    lsb = crc & 0xFF
    for c in data:
        x = ord(c) ^ msb
        x ^= (x >> 4)
        msb = (lsb ^ (x >> 3) ^ (x << 4)) & 0xFF
        lsb = (x ^ (x << 5)) & 0xFF
    crc = (msb << 8) + lsb
    return crc


# 9600, 8N1
ser = serial.Serial(
    port = sys.argv[1], # e.g. /dev/ttyACM0 or /dev/ttyUSB0
    baudrate = 9600,
    bytesize=serial.EIGHTBITS,
    stopbits = serial.STOPBITS_ONE,
    parity = serial.PARITY_NONE,
    timeout = 10
)

line = None

# Throw everything away until we see the STARTING line
while True:
    line = ser.readline()
    if "STARTING" in line:
        break

# The first line will be the CSV header, so don't add a timestamp to this line.
line = ser.readline()
# shave off the 'crc16' header column, add a 'timestamp' column
line = "timestamp," + line.rstrip().rsplit(',', 1)[0]
sys.stdout.write(line + '\n')
sys.stdout.flush()

while True:
    line = ser.readline().rstrip()
    (data, crc) = line.rsplit(',', 1)
    if crc16_xmodem(data) != int(crc, 16):
        sys.stderr.write("WARNING: skipping line with bad CRC16")
        continue
    sys.stdout.write("%.2f,%s\n" % (time.time(), data))
    sys.stdout.flush()
