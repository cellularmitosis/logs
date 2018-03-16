#!/bin/bash

set -e -o pipefail

now=`date +%s`
subdir="${1}-${now}"
mkdir -p ${subdir}

./Si7021-logger.py /dev/ttyACM0 >> ${subdir}/ambient.csv &
echo $! >> ${subdir}/pids

