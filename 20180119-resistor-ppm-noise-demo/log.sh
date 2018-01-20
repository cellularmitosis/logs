#!/bin/bash

set -e -o pipefail

test -n "${1}" || exit 1
mkdir -p "${1}"
./hp-and-env-logger.py /dev/ttyACM0 /dev/ttyUSB0 | tee "${1}"/out.csv
