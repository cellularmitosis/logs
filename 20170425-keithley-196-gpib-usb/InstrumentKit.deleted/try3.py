#!/usr/bin/env python

import instruments
import sys

k196 = instruments.keithley.Keithley196.open_gpibusb(sys.argv[1], 7)
print k196.measure(k196.Mode.resistance)
#print k196.query('')
