#!/bin/bash

set -e -o pipefail

./trivial-serial.py /dev/tty.usbmodem1421 | tee tune.csv

# followed by e.g.
# cat tune.csv | grep -v debug > run2/tune.csv
