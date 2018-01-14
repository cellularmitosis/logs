#!/bin/bash

set -e -o pipefail

./LTC2400-logger.py /dev/ttyACM0 | tee `date +%s`-LTC2400.csv
