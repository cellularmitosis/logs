#!/usr/bin/env python3

# logger.py: read CSV data from an Arduino.
#
# usage examples:
# ./logger.py /dev/ttyACM0 | tee log.txt
# ./logger.py /dev/tty.usbmodem14101 | tee log.txt
# ./logger.py /dev/tty.usbserial-1110 | tee log.txt

import sys
import serial  # pip3 install pyserial

if __name__ == "__main__":
    if len(sys.argv) < 2:
        sys.stdout.write("Usage: ./logger.py <device>\n")
        sys.stdout.write("e.g. (Linux): ./logger.py /dev/ttyACM0 | tee log.txt\n")
        sys.stdout.write("e.g. (macOS): ./logger.py /dev/tty.usbmodem14101 | tee log.txt\n")
        sys.exit(1)

    # 9600, 8N1
    ser = serial.Serial(
        port = sys.argv[1], # e.g. /dev/ttyACM0 or /dev/ttyUSB0
        baudrate = 9600,
        bytesize=serial.EIGHTBITS,
        stopbits = serial.STOPBITS_ONE,
        parity = serial.PARITY_NONE,
        timeout = 3
    )

    # Discard the startup message.
    while True:
        line = ser.readline().decode('utf-8')
        if "CSV output:" in line:
            break

    while True:
        line = ser.readline().decode('utf-8')
        sys.stdout.write(line)
        sys.stdout.flush()
