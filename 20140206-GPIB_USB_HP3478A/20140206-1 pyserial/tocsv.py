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

#s.write("+reset\r"); time.sleep(0.1)
s.write("+a:23\r"); time.sleep(0.1)
s.write("+eoi:0\r"); time.sleep(0.1)
s.write("+eos:13\r"); time.sleep(0.1)

start = time.time()

while True:
    value = requestHPline(s)
    now = time.time() - start
    print "%s, %s" % (now, value)


s.close()
sys.exit()
