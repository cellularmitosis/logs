#!/usr/bin/env python

# csv-cull.py: strip columns out of a CSV file.
# specify the column indexes you want to keep.
# $ cat input.csv | ./csv-cull.py 0,1,3 > output.csv

import csv
import sys

# adapted from https://stackoverflow.com/a/7589615

cols = [int(col) for col in sys.argv[1].split(',')]

reader = csv.reader(sys.stdin)
writer = csv.writer(sys.stdout)
for r in reader:
    values = [r[i] for i in cols]
    writer.writerow(values)
