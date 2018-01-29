#!/bin/bash

set -e -o pipefail

test -n "${1}"

now=`date +%s`
mkdir -p "${now}-${1}"
./serial-log.py /dev/tty.usbmodem1421 | tee "${now}-${1}/out.csv"

