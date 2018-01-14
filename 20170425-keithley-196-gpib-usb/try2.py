#!/usr/bin/env python

import serial
import time, datetime
import sys

def bprint(msg):
	sys.stdout.write("%s bytes: '" % len(msg))
	for ch in msg:
		if ord(ch) == 9:
			sys.stdout.write(r"\t")
		elif ord(ch) == 10:
			sys.stdout.write(r"\n")
		elif ord(ch) == 13:
			sys.stdout.write(r"\r")
		elif ord(ch) < 32:
			sys.stdout.write("%%%s" % ord(ch))
		elif ord(ch) < 127:
			sys.stdout.write(ch)
		else:
			sys.stdout.write("%%%s" % ord(ch))
	sys.stdout.write("'\n")

def bsend(msg):
	sys.stdout.write("\nsending ")
	bprint(msg)
	ser.flushInput() # this doesn't flush, it clears the input buffer
	ser.flushOutput() # this doesn't flush, it clears the output buffer
	ser.write(msg)

def breadline():
	sys.stdout.write("read ")
	ser.timeout = 2
	reply = ser.readline()
	bprint(reply)
	return reply

# connect to the Galvant Industries GPIB-USB adapter
# on linux, try e.g. /dev/ttyUSB0
# on mac, try e.g. /dev/tty.usbserial-DB00F6OG
ser = serial.Serial(sys.argv[1], 460800, 8, 'N', 1)

# assert that the GPIB adapter is working
bsend("+test\r")
reply = breadline()
assert reply == "testing\n"

# assert that the GPIB-USB adapter is running firmware v4
bsend("+ver\r")
reply = breadline()
assert reply == "4\r"

# enable debug mode on the GPIB-USB adapter
bsend("+debug:0\r")
time.sleep(0.1)

# turn off autoread.  we will manually tell the adapter when to read.
#bsend("+autoread:0\r")
#time.sleep(0.1)

# tell the adapter to talk to IEEE-488 address 7
#bsend("+a:7\r")
#time.sleep(0.1)

# read the status register from the DMM
#bsend("U0DX\r")
#bsend("U0DX\n")
#bsend("U0DX\r\n")
#bsend("U0DX\n\r")
bsend("U7X\r")
time.sleep(0.1)
#bsend("+read\r")
reply = breadline()

ser.close()
sys.exit(0)

print "sending: +ver"
ser.write("+ver\r")
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
