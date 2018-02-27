#!/usr/bin/env python

import serial
import sys
import time

ser = serial.Serial(
    port = sys.argv[1],
    baudrate = 9600,
    bytesize=serial.EIGHTBITS,
    stopbits = serial.STOPBITS_TWO,
    parity = serial.PARITY_NONE,
    timeout = 10
)

sys.stdout.write("timestamp,v\n")

current = None
upper = 18.0
lower = 9.0
step = 0.1
period = 60

while True:
    current = upper
    while current >= lower:
        ser.write("v%0.1f;" % current)
        sys.stdout.write("%0.1f,%0.1f\n" % (time.time(), current))
        sys.stdout.flush()
        time.sleep(period)
        current -= step

    current = lower
    while current <= upper:
        ser.write("v%0.1f;" % current)
        sys.stdout.write("%0.1f,%0.1f\n" % (time.time(), current))
        sys.stdout.flush()
        time.sleep(period)
        current += step

