#!/bin/bash

set -e

arduino=/dev/ttyUSB1
dmm=/dev/ttyUSB0
./multi-logger.py $arduino $dmm
