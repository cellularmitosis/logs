#!/bin/bash

set -e

arduino=/dev/tty.usbmodem1411
dmm=/dev/tty.usbserial-AH05Y104
./log-k2015.py $arduino $dmm
