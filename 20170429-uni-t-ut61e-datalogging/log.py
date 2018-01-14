#!/usr/bin/env python

# sudo pip install ut61e
# sudo pip install pyserial

import serial
import sys

ser = serial.Serial(
    port = "/dev/ttyUSB0",
    baudrate = 19200,
    bytesize=serial.SEVENBITS,
    stopbits = serial.STOPBITS_ONE,
    parity = serial.PARITY_ODD,
    timeout=15
)

while True:
    byte = ser.read(1)
    sys.stdout.write(byte)
    sys.stdout.flush()
