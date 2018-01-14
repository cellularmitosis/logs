#!/bin/bash

set -e

mkdir -p snap1
head -n1 1500068880-LTC2400.csv > snap1/log.csv
#tail -n +48 1500068880-LTC2400.csv >> snap1/log.csv
tail -n +250 1500068880-LTC2400.csv >> snap1/log.csv
./plot-LTC2400.py snap1/log.csv
#ristretto snap1/log.png
