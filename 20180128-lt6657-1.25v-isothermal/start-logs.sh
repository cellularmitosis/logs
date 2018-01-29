#!/bin/bash

set -e -o pipefail

test -n "${1}"

now=`date +%s`
subdir="${now}-${1}"
mkdir -p "${subdir}"

#./34401A-listen.py /dev/ttyUSB0 lt6657 >> "${subdir}/data.csv" &
./34401A-listen.py /dev/tty.usbserial-AH05Y104 lt6657 > "${subdir}/lt6657.csv" &
echo $! >> "${subdir}"/pids

./serial-log.py /dev/tty.usbmodem1411 > "${now}-${1}/isothermal.csv" &
echo $! >> ${subdir}/pids

