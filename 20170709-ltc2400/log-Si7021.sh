#!/bin/bash

set -e -o pipefail

./Si7021-logger.py /dev/ttyACM1 | tee `date +%s`-Si7021.csv
