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

sys.stdout.write("timestamp,%s\n" % sys.argv[2]); sys.stdout.flush()

last_sample_timestamp = time.time()
initial_skips = 0

while True:
    line = ser.readline()
    now = time.time()
    elapsed = now - last_sample_timestamp
    last_sample_timestamp = now

    # The FTDI chip seems to buffer up a certain amount of outgoing bytes if
    # there is no listener to recieve them.  Thus, when we first run this
    # script, we will get an initial flood of queued readings.  Throw those
    # away, because we don't have a timestamp for them.
    if elapsed < 0.3:
        continue

    # Additionally, it seems we still have to throw away the next 1 sample.
    if initial_skips < 1:
        initial_skips += 1
        continue

    ohms = float(line.rstrip())
    sys.stdout.write("%0.2f,%0.3f\n" % (now, ohms)); sys.stdout.flush()
