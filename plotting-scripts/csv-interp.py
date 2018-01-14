#!/usr/bin/env python

# csv-combine.py: pull columns from 2 CSV files (using linear interpolation for the second file).
# usage: ./csv-combine.py fileA.csv <column> fileB.csv <column>
# $ ./csv-combine.py A.csv 1 B.csv 1 > output.csv

import csv
import sys

# adapted from https://stackoverflow.com/a/7589615

a_fname = sys.argv[1]
a_col = int(sys.argv[2])

b_fname = sys.argv[3]
b_col = int(sys.argv[4])

a_reader = csv.reader(open(a_fname))
b_reader = csv.reader(open(b_fname))
writer = csv.writer(sys.stdout)

a_titles = a_reader.next()
b_titles = b_reader.next()

row = (a_titles[0], a_titles[a_col], b_titles[b_col])
writer.writerow(row)

b1 = b_reader.next()
b1_date = float(b1[0])
b1_val = float(b1[b_col])

b2 = b_reader.next()
b2_date = float(b1[0])
b2_val = float(b2[b_col])

for a in a_reader:
    a_date = float(a[0])
    a_val = float(a[a_col])
    # advance the A reader until we are in B's data range
    if a_date < b1_date:
        continue
    # advance the B reader until we straddle the current A point
    while a_date > b2_date:
        b1 = b2
        b1_date = b2_date
        try:
            b2 = b_reader.next()
        except StopIteration:
            sys.exit(0)
        b2_date = float(b1[0])
    frac = b2_date - b1_date
    b_val = b1_val + frac * (b2_val - b1_val)
    row = (a[0], a[a_col], b_val)
    writer.writerow(row)
