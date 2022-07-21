#!/usr/bin/env python

# Apply a "Savitzky-Golay" smoothing filter to the data.

import sys
import pandas as pd
import scipy.signal

df = pd.read_csv(sys.argv[-1])
window=151
order=2
df["Savitzky-Golay (%s,%s)" % (window, order)] = scipy.signal.savgol_filter(df["PPM"], window, order)
df.to_csv("savgol.csv", index=False)
