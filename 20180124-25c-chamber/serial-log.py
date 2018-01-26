#!/usr/bin/env python

import sys
import serial
import time

if __name__ == "__main__":
    # 9600, 8N1
    ser = serial.Serial(
        port = sys.argv[1], # e.g. /dev/ttyACM0 or /dev/ttyUSB0
        baudrate = 9600,
        bytesize=serial.EIGHTBITS,
        stopbits = serial.STOPBITS_ONE,
        parity = serial.PARITY_NONE,
        timeout = 10
    )

    last_sample_timestamp = time.time()

    while True:
        # the FTDI chip seems to buffer up a certain amount of outgoing bytes if there is no
        # listener to recieve them.  thus, when we first run this script, we will get an initial
        # flood of queued readings.  throw those away.
        line = ser.readline()
        now = time.time()
        if now - last_sample_timestamp < 0.1:
            continue
        last_sample_timestamp = now

        sys.stdout.write(line)
        sys.stdout.flush()
