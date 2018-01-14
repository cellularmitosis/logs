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

sys.stdout.write("timestamp,value\n"); sys.stdout.flush()

last_sample_timestamp = time.time()

while True:
    line = ser.readline()
    now = time.time()

    # The FTDI chip seems to buffer up a certain amount of outgoing bytes if
    # there is no listener to recieve them.  Thus, when we first run this
    # script, we will get an initial flood of queued readings.  Throw those
    # away, because we don't have a timestamp for them.
    if now - last_sample_timestamp < 0.1:
        continue
    last_sample_timestamp = now

    microvolts = float(line.rstrip()) * 1000000
    sys.stdout.write("%0.3f,%0.1f\n" % (now, microvolts)); sys.stdout.flush()
