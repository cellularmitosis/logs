import sys
import time
import serial


def readHPline(fd=None, maxchars=1024):
    buffer = ''
    for i in range(maxchars):
	ch = fd.read(1)
	if len(ch) == 0 or ord(ch) == 13:
	    break
	buffer = buffer + ch
    return buffer.strip()


def requestHPline(fd):
    fd.write("+read\r")
    return readHPline(fd)


# this was written to run under cygwin, using the Galvant GPIB USB adapter
s = serial.Serial(port='/dev/com3', baudrate='460800', timeout=10)

s.write("+a:23\r")
s.write("+eoi:0\r")
s.write("+eos:13\r")

print "starting..."


print "\nsetting DC Volts, 5.5 digits, internal trigger"
s.write("F1N5T1\r")
time.sleep(0.1)

print "measurement: ",
print requestHPline(s)
time.sleep(0.1)

print "measurement: ",
print requestHPline(s)
time.sleep(0.1)


print "\nsetting AC Volts, 5.5 digits, internal trigger"
s.write("F2N5T1\r")
time.sleep(0.1)

print "measurement: ",
print requestHPline(s)
time.sleep(0.1)

print "measurement: ",
print requestHPline(s)
time.sleep(0.1)


print "\nsetting DC Volts, 4.5 digits, internal trigger"
s.write("F1N4T1\r")
time.sleep(0.1)

print "reading 10 measurements quickly:"
for i in range(10):
    print requestHPline(s)


print "bye..."


s.close()
sys.exit()
