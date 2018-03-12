#!/bin/bash

set -e -o pipefail

test -n "${1}"
dest="${1}"
mkdir -p "${dest}"
cd "${dest}"
arduino=/dev/ttyACM1
#arduino=/dev/ttyUSB1
hp=/dev/ttyUSB0
../multi-logger.py $arduino $hp
../savgol.py tempco.csv
mv savgol.csv tempco.csv
play ../../plotting-scripts/airhorn.wav
