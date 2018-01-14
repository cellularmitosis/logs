#!/usr/bin/env python

import sys

while True:
    line = sys.stdin.readline()
    volts = float(line.split(';')[1])
    millivolts = volts * 1000
    degrees_c = (millivolts - 400) / 19.5
    degrees_f = degrees_c * 1.8 + 32
    print "%.02fF" % (degrees_f)
    sys.stdout.flush()


