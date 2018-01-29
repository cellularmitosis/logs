#!/bin/bash

set -e -o pipefail

test -n "${1}"

now=`date +%s`
subdir="${1}-${now}"
mkdir -p "${subdir}"

./multi-logger.py /dev/tty.usbserial-AH05Y104 /dev/tty.usbmodem1411 > "${subdir}/out.csv" &
echo $! >> ${subdir}/pids
