#!/bin/bash

set -e -o pipefail

cd "${1}"

cat env.csv | ../csv-cull.py 0,1 > temp_c.csv
cat env.csv | ../csv-cull.py 0,2 > humidity.csv

