#!/bin/bash

set -e -o pipefail

now=`date +%s`
subdir="${1}-${now}"
mkdir -p ${subdir}

#device=/dev/ttyACM0
device=/dev/tty.usbmodem1411
./Si7021-logger.py $device >> ${subdir}/ambient.csv &
echo $! >> ${subdir}/pids

