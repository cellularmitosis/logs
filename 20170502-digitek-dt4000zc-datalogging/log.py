#!/usr/bin/env python

import tp4000zc

dmm = tp4000zc.Dmm('/dev/ttyUSB0')


while True:
    val = dmm.read()
    print val.text
#    print val.numericVal

dmm.close()

