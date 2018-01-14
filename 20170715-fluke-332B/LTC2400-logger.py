#!/usr/bin/env python

import serial
import sys
import time

ser = serial.Serial(
    port = sys.argv[1],
    baudrate = 115200,
    bytesize=serial.EIGHTBITS,
    stopbits = serial.STOPBITS_ONE,
    parity = serial.PARITY_NONE,
    timeout = 10
)

sys.stdout.write("timestamp,value\n")
sys.stdout.flush()

# globals
last_sample_timestamp = time.time()

while True:

    line = ser.readline()

#    if len(line) != 10:
#        continue
    
    now = time.time()

    # the FTDI chip seems to buffer up a certain amount of outgoing bytes if there is no
    # listener to recieve them.  thus, when we first run this script, we will get an initial
    # flood of queued readings.  throw those away.
    if now - last_sample_timestamp < 0.1:
        continue
    last_sample_timestamp = now

    # print out the results in CSV format
    microvolts = float(line.rstrip()) * 1000000
    sys.stdout.write("%0.3f,%0.0f\n" % (now, microvolts))
    sys.stdout.flush()
