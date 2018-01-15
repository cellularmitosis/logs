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

#    ser.write("*RST\r\n")
#    time.sleep(3)
    ser.write("SENSe:FUNCtion 'FRESistance'\n") # 4-wire resistance mode
    time.sleep(1)
    ser.write(":SENSe:FRESistance:NPLCycles 10\n") # slowest measurement rate
    time.sleep(1)
#    ":TRIGger:COUNt 1"
#    ":INITiate"
#    "*TRG"


    while True:
        #ser.write(":SENSe:DATA:FRESh?\n")
        ser.write(":READ?\n")
        sys.stdout.write(ser.read())
        sys.stdout.flush()

# I give up.  The keithley is constantly complaining about "error -363", yet it also
# gives me the readings I ask for.  strange.
