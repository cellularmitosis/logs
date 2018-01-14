#!/usr/bin/env python

# Python 2.7 code to get readings from Keithley 196 multimeter via GPIB
# Assumes meter is already set up to correct mode, eg. from front panel
# Dec. 17 2012 J.Beale

from __future__ import print_function   # to use print without newline
from serial import *
import time,datetime

cmd = "?\r"            # GPIB command to Keithley 196 must end with carriage return
eol_str = "\n"  # end of line string in file output
buffer =''           # serial input buffer
outbuf = ''          # file output buffer

import sys
ser=Serial(sys.argv[1],460800,8,'N',1,timeout=1)  # GPIB-USB board on COM18, with 460800 bps, 8-N-1
f = open('K196-log.csv', 'w')
print ("Keithley 196 log v0.1 Dec.17 2012 JPB")
f.write ("date_time,volts\n")
f.write ("# Keithley 196 log v0.1 Dec.17 2012 JPB\n")

while True:
    ser.read(255)            # clear out existing buffer & throw it away
    ser.write(cmd)                 # send query to instrument
    buf = ser.readline()         # string terminated with '\n'
    buffer = buf.split()[0]   # get rid of the \r\n characters at the end
    outbuf = str(datetime.datetime.now()) + ',' + buffer
    print (outbuf)
    f.write (outbuf)
    f.write (eol_str)
    time.sleep(9)                        # from 'time' to wait this many seconds
    
f.close                  # close log file
ser.close()            # close serial port when done. If we ever are...
