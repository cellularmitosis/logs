#!/usr/bin/env python

# Apply a "Savitzky-Golay" smoothing filter to the data.

import sys
import pandas as pd
import scipy.signal

df = pd.read_csv(sys.argv[-1])
df["savgol_151_2"] = scipy.signal.savgol_filter(df["ppm"], 151, 2)
df.to_csv("savgol.csv", index=False)
