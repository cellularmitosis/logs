#!/usr/bin/env python

import tp4000zc
import datetime
import sys

dmm = tp4000zc.Dmm('/dev/ttyUSB1')

print "Date,Amps"

while True:
    val = dmm.read()
    amps = val.numericVal
    if amps is None:
        continue
    now = datetime.datetime.now()
    print "%s,%f" % (now, amps)
    sys.stdout.flush()

dmm.close()

