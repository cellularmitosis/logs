#!/usr/bin/env python

import sys
import serial
import time
import os

if __name__ == "__main__":

    if len(sys.argv) < 2:
        sys.stderr.write("usage: %s <Arduino>\n" % sys.argv[0])
        sys.stderr.write("e.g. %s COM1\n" % sys.argv[0])
        sys.exit(1)

    # 9600, 8N1
    arduino = serial.Serial(
        port = sys.argv[1], # e.g. /dev/ttyACM0, /dev/ttyUSB0, COM1, etc.
        baudrate = 9600,
        bytesize=serial.EIGHTBITS,
        stopbits = serial.STOPBITS_ONE,
        parity = serial.PARITY_NONE,
        timeout = 10
    )

    # The Arduino will start by spitting out a few empty lines, then the CSV
    # header, then the readings.  So throw away the input until we find an
    # empty and then non-empty line.
    while True:
        line = arduino.readline()
        if line == "\n":
            while True:
                line = arduino.readline()
                if line != "\n":
                    arduino_csv_header = line.rstrip()
                    break
            break

    if os.path.exists("log.csv"):
        sys.stderr.write("Error: refusing to clobber existing log.csv!\n")
        sys.exit(1)
    csv = open("log.csv", "w")
    csv.write("timestamp,temp_c,rh\n")
    csv.flush()

    while True:
        line = arduino.readline()
        if line is None:
            time.sleep(0.01)
            continue
        temp_c = float(line.rstrip().split(",")[0])
        humidity = float(line.rstrip().split(",")[1])
        csv.write("%s,%s,%s\n" % (time.time(), temp_c, humidity))
        csv.flush()
