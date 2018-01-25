#!/bin/bash

set -e -o pipefail

now=`date +%s`
test -n "${now}-${1}"
mkdir -p "${now}-${1}"
./trivial-serial.py /dev/tty.usbmodem1411 | tee "${now}-${1}/out.csv"

