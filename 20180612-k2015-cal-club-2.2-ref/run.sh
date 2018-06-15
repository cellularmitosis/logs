#!/bin/bash

set -e

arduino=/dev/ttyUSB0
dmm=/dev/ttyUSB1
./log-k2015.py $arduino $dmm
