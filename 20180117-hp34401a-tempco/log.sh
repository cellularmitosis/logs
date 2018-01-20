#!/bin/bash

set -e -o pipefail

./hp-and-env-logger.py /dev/ttyACM0 /dev/ttyUSB0 | tee out.csv
