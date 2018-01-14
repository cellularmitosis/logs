#!/bin/bash

set -e -o pipefail

nice ./plot-LTC2400.py -s $1 -m $2 1500148011-LTC2400.csv /tmp/out.png
#nice ./plot-LTC2400.py -s $1 -m 1 1500145015-Si7021.csv /tmp/temp.png
