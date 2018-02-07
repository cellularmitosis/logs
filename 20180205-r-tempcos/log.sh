#!/bin/bash

set -e -o pipefail

./multi-logger.py /dev/tty.usbmodem1421 /dev/tty.usbserial-AH05Y104
./savgol.sh tempco.csv
mv savgol.csv tempco.csv
