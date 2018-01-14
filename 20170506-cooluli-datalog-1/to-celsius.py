#!/usr/bin/env python

import sys

while True:
    line = sys.stdin.readline()
    volts = float(line.split(';')[1])
    millivolts = volts * 1000
    degrees_c = (millivolts - 400) / 19.5
    print "%.03fC" % (degrees_c)
    sys.stdout.flush()


