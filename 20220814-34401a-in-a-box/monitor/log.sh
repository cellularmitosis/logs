#!/bin/bash

set -e -o pipefail

if test -n "$1"; then
    fname="log-$1.csv"
else
    fname="log-$(date +%s).csv"
fi

echo "Logging to $fname"
./logger.py /dev/tty.usb* | tee "$fname"
