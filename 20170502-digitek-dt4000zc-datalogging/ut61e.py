#!/usr/bin/env python

import serial

ser = serial.Serial(
    port='/dev/ttyUSB0',
    baudrate=19200,
    parity=serial.PARITY_ODD,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.SEVENBITS
)

while True:
    val = dmm.read()
    print val.text
#    print val.numericVal

dmm.close()

