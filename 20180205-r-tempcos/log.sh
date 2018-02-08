#!/bin/bash

set -e -o pipefail

test -n "${1}"
dest="${1}"
mkdir -p "${dest}"
cd "${dest}"
../multi-logger.py /dev/tty.usbmodem1421 /dev/tty.usbserial-AH05Y104
../savgol.py tempco.csv
mv savgol.csv tempco.csv
