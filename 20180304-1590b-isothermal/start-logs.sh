#!/bin/bash

set -e -o pipefail

now=`date +%s`
subdir="${1}-${now}"
mkdir -p ${subdir}

#./34401A-ohms.py /dev/ttyUSB0 thermistor >> ${subdir}/thermistor.csv &
#echo $! >> ${subdir}/pids

./Si7021-logger.py /dev/ttyACM0 >> ${subdir}/isothermal.csv &
echo $! >> ${subdir}/pids

./Si7021-logger.py /dev/ttyACM1 >> ${subdir}/ambient.csv &
echo $! >> ${subdir}/pids

