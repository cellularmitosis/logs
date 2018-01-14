#!/bin/bash

set -e -o pipefail

./Si7021-logger.py /dev/ttyACM0 | tee `date +%s`.csv
