#!/usr/bin/env python

import sys
import csv

# read in the csv rows
rows = []
with open(sys.argv[1], "r") as fd:
    for row in csv.reader(fd):
        rows.append(row)



# read the voltage readings from the second column
volts = [row[1] for row in rows]

# read the temperature readings from the third column
degrees_f = [row[2] for row in rows]

