#!/usr/bin/env python

# Python 2.7 code to get readings from Keithley 196 multimeter via GPIB
# Assumes meter is already set up to correct mode, eg. from front panel
# Dec. 17 2012 J.Beale

#from __future__ import print_function   # to use print without newline
import serial
import time, datetime
import sys

cmd = "?\r"            # GPIB command to Keithley 196 must end with carriage return
eol_str = "\n"  # end of line string in file output
buffer =''           # serial input buffer
outbuf = ''          # file output buffer

ser = serial.Serial(sys.argv[1], 460800, 8, 'N', 1, timeout=1)  # GPIB-USB board on COM18, with 460800 bps, 8-N-1
#f = open('K196-log.csv', 'w')
#print ("Keithley 196 log v0.1 Dec.17 2012 JPB")
#f.write ("date_time,volts\n")
#f.write ("# Keithley 196 log v0.1 Dec.17 2012 JPB\n")

print "sending: +test"
ser.write("+test\r")
time.sleep(0.1)
print "->%s<-" % ser.readline()
print

print "sending: +debug:1"
ser.write("+debug:1\r")
time.sleep(0.1)
print "->%s<-" % ser.readline()
print

print "sending: +ver"
ser.write("+ver\r")
time.sleep(0.1)
print "->%s<-" % ser.readline()
print

print "sending: +a:7"
ser.write("+a:7\r")
time.sleep(0.1)
print "->%s<-" % ser.readline()
print

# one-shot on GET
print "sending: T3X"
ser.write("T3X\r")
time.sleep(0.1)
print "->%s<-" % ser.readline()
print

print "sending: +get"
ser.write("+get\r")
time.sleep(0.1)
print "->%s<-" % ser.readline()
print

ser.close()
sys.exit(0)

print "sending: F3X;"
ser.write("F3X;\r")
time.sleep(0.1)
print "->%s<-" % ser.readline()
print

print "sending: X;"
ser.write("X;\r")
time.sleep(0.1)
print "->%s<-" % ser.readline()
print

print "sending: ?"
ser.write("?\r")
time.sleep(0.1)
print "->%s<-" % ser.readline()
print

print "sending: +read"
ser.write("+read\r")
time.sleep(0.1)
print "->%s<-" % ser.readline()
time.sleep(0.1)
print "->%s<-" % ser.readline()
time.sleep(0.1)
print "->%s<-" % ser.readline()
time.sleep(0.1)
print "->%s<-" % ser.readline()
time.sleep(0.1)
print "->%s<-" % ser.readline()
time.sleep(0.1)
print "->%s<-" % ser.readline()
time.sleep(0.1)
print "->%s<-" % ser.readline()
time.sleep(0.1)
print "->%s<-" % ser.readline()
print


# time.sleep(1)

# ser.read(255)
# print "sending: ?"
# ser.write("?\r")
# buf = ser.readline()
# print "read %s bytes:" % len(buf)
# print buf
# print

# print "sending: F3X"
# ser.write("F3X\r")
# time.sleep(0.1)
# print "->%s<-" % ser.readline()
# print

# print "sending: ?"
# ser.write("?\r")
# time.sleep(0.1)
# print "->%s<-" % ser.readline()

# print "sending: *IDN?;"
# ser.write("?;\r")
# time.sleep(0.1)
# print "->%s<-" % ser.readline()
# print

# print "sending: DhX"
# ser.write("DhX\r")
# time.sleep(0.1)
# print "->%s<-" % ser.readline()


#while True:
#    print "->%s<-" % ser.read(1)            # clear out existing buffer & throw it away
#    print(buf)
 #   buffer = buf.split()[0]   # get rid of the \r\n characters at the end
  #  outbuf = str(datetime.datetime.now()) + ',' + buffer
  #  print(outbuf)
  #  f.write (outbuf)
  #  f.write (eol_str)
  #  time.sleep(9)                        # from 'time' to wait this many seconds
    
#f.close                  # close log file
ser.close()            # close serial port when done. If we ever are...
