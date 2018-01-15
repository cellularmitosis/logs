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

    while True:
        sys.stdout.write(ser.readline())
        sys.stdout.flush()
