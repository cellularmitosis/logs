#!/bin/bash

set -e -o pipefail

now=`date +%s`
subdir="${1}-${now}"
mkdir -p ${subdir}

#./34401A-100mv.py /dev/ttyUSB0 $1 >> ${subdir}/data.csv &
./34401A-100mv.py /dev/tty.usbserial-AH05Y104 $1 >> ${subdir}/data.csv &
echo $! >> ${subdir}/pids

#./Si7021-logger.py /dev/ttyACM0 >> ${subdir}/env.csv &
./Si7021-logger.py /dev/tty.usbmodem1421 >> ${subdir}/env.csv &
echo $! >> ${subdir}/pids
