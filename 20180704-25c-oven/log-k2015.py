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
                    # "line" was "STARTING".  next line is the csv header.
                    arduino_csv_header = arduino.readline().rstrip()
                    break
            break

    if os.path.exists("log.csv"):
        sys.stderr.write("Error: refusing to clobber existing log.csv!\n")
        sys.exit(1)
    csv = open("log.csv", "w")
    csv.write("timestamp,v,temp_c\n")
    csv.flush()

    setup_cmds = [
        "*RST",
        ":INITiate:CONTinuous ON",
        ":ABORt",
        ":SYSTem:AZERo:STATe ON",
        ":CONFigure:VOLTage:DC",
       ":SENSe:VOLTage:DC:RANGe 10",
        ":SENSe:VOLTage:DC:NPLC 4",
        # ":SENSe:VOLTage:DC:AVERage:TCONTrol REPeat",
        ":SENSe:VOLTage:DC:AVERage:TCONTrol MOVing",
        ":SENSe:VOLTage:DC:AVERage:COUNt 10",
        ":SENSe:VOLTage:DC:AVERage:STATe ON",
    ]

    loop_cmds = [
        ":READ?",
    ]

    for cmd in setup_cmds:
        dmm.write(cmd + "\n")

    last_temp_c = None

    # Strategy: record a line of output for every reading from the dmm.
    # Use whatever the last available temperature reading was.
    while True:
        if arduino.inWaiting():
            line = arduino.readline()
            last_temp_c = float(line.rstrip().split(",")[0])

        for cmd in loop_cmds:
            dmm.write(cmd + "\n")
        line = dmm.readline()
        dmm_value = float(line.rstrip())

        if last_temp_c is None:
            time.sleep(0.01)
            continue

        csv.write("%s,%s,%s\n" % (time.time(), dmm_value, last_temp_c))
        csv.flush()
