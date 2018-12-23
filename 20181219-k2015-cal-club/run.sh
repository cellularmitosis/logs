#!/bin/bash

set -e

# linux:
arduino=/dev/ttyUSB0
dmm=/dev/ttyUSB1

# mac:
#arduino=/dev/tty.usbmodem1411
#arduino=/dev/tty.wchusbserial1410
#arduino=/dev/tty.wchusbserial1420
#dmm=/dev/tty.usbserial-AH05Y104

./log-k2015.py $arduino $dmm
