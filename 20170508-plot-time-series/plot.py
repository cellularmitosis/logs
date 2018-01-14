#!/usr/bin/env python

# debian: sudo apt-get install python-matplotlib python-dateutil

import matplotlib.pyplot as plt
import dateutil
import sys
import csv

# read in the csv rows
rows = []
with open("temp.csv", "r") as fd:
    for row in csv.reader(fd):
        rows.append(row)

# throw away the header
rows = rows[1:]

# slice out just the first 300 data points
rows = rows[:300]

# parse the dates from the first column
dates = [dateutil.parser.parse(row[0]) for row in rows]

# read the voltage readings from the second column
volts = [row[1] for row in rows]

# read the temperature readings from the third column
degrees_f = [row[2] for row in rows]

# create a 2D plot of date vs temperature
plt.plot(dates, degrees_f)

# display the plot in a window
plt.show()

