#!/usr/bin/env python

import sys
import serial
import time

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
sys.stdout.write(line)
sys.stdout.flush()

while True:
    line = ser.readline()
    sys.stdout.write("%.2f,%s" % (time.time(), line))
    sys.stdout.flush()
