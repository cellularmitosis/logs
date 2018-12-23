#!/usr/bin/env python

import sys
import serial
import time
import os

if __name__ == "__main__":

    if len(sys.argv) < 3:
        sys.stderr.write("usage: %s <Arduino> <DMM>\n" % sys.argv[0])
        sys.stderr.write("e.g. %s /dev/ttyUSB0 /dev/ttyUSB1\n" % sys.argv[0])
        sys.exit(1)

    # 9600, 8N1
    arduino = serial.Serial(
        port = sys.argv[1], # e.g. /dev/ttyACM0 or /dev/ttyUSB0
        baudrate = 9600,
        bytesize=serial.EIGHTBITS,
        stopbits = serial.STOPBITS_ONE,
        parity = serial.PARITY_NONE,
        timeout = 10
    )

    # 9600, 8N1
    dmm = serial.Serial(
        port = sys.argv[2], # e.g. /dev/ttyACM0 or /dev/ttyUSB0
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
    csv.write("timestamp,v,temp_c\n")
    csv.flush()

    # some USB-serial chips seems to buffer up a certain amount of outgoing bytes if there is no
    # listener to recieve them.  thus, when we first run this script, we will get an initial
    # flood of queued readings.  throw those away.
    then = time.time()
    dmm.readline()
    now = time.time()
    while now - then < 0.2:
        then = time.time()
        dmm.readline()
        now = time.time()

    last_temp_c = None

    # Strategy: record a line of output for every reading from the dmm.
    # Use whatever the last available temperature reading was.
    while True:
        if arduino.inWaiting():
            line = arduino.readline()
            last_temp_c = float(line.rstrip().split(",")[0])

        if dmm.inWaiting():
            dmm_value = float(dmm.readline().rstrip())
            if last_temp_c is None:
                time.sleep(0.01)
                continue
            csv.write("%s,%s,%s\n" % (time.time(), dmm_value * 1000000, last_temp_c))
            csv.flush()

        time.sleep(0.01)
