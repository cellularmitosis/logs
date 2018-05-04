#!/bin/bash

set -e

arduino=/dev/ttyUSB1
dmm=/dev/ttyUSB0
./log-k2015.py $arduino $dmm
