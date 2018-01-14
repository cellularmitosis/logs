#!/usr/bin/env python

import tp4000zc
import datetime
import sys

dmm = tp4000zc.Dmm('/dev/ttyUSB0')

print "Date,Volts,Farenheit"

while True:
    val = dmm.read()
    volts = val.numericVal
    millivolts = volts * 1000
    degrees_c = (millivolts - 400) / 19.5
    degrees_f = degrees_c * 1.8 + 32
    now = datetime.datetime.now()
    print "%s,%.03f,%.02f" % (now, volts, degrees_f)
    sys.stdout.flush()

dmm.close()

