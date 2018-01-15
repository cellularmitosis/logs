#!/bin/bash

mkdir -p "${1}"
cd "${1}"
../multi-logger.py /dev/tty.usbmodem1421 /dev/tty.usbserial-AH05Y104
