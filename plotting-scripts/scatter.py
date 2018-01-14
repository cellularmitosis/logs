#!/usr/bin/env python

import sys

import matplotlib
# Force matplotlib to not use any Xwindows backend.
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv(sys.argv[1])
plot = plt.scatter(df[sys.argv[2]], df[sys.argv[3]])
fig = plot.get_figure()
fig.savefig('scatter.png')

