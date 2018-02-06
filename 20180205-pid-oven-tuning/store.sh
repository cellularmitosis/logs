#!/bin/bash

set -e -o pipefail

test -n "${1}"

mkdir -p "${1}"
cat tune.csv | grep -v debug > "${1}/tune.csv"
rm tune.csv
