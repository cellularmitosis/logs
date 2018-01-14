import sys
import time
import serial


def readHPline(fd=None, maxchars=1024):
    then = time.time()
    
    buffer = ''
    for i in xrange(maxchars):
	ch = fd.read(1)
	if len(ch) == 0 or ord(ch) == 13:
	    break
	buffer = buffer + ch
	
    now = time.time()
    elapsed = now - then
    
    return (elapsed, buffer.strip())


def requestHPline(fd):
#    time.sleep(0.05)
    fd.write("+read\r"); #time.sleep(0.05)
    return readHPline(fd)


# this was written to run under cygwin, using the Galvant GPIB USB adapter
s = serial.Serial(port='/dev/com3', baudrate='460800', timeout=10)
time.sleep(0.05)

#s.write("+reset\r"); time.sleep(0.05)
s.write("+a:23\r"); time.sleep(0.05)
s.write("+eoi:0\r"); time.sleep(0.05)
s.write("+eos:13\r"); time.sleep(0.05)

overall_then = time.time()

i = 0
while True:
    i += 1

    then = time.time()
    
    line = requestHPline(s)
    
    now = time.time()
    elapsed = now - then

#    if i % 10 == 0:
#        print line, "total:", elapsed
#    sys.stdout.write("."); sys.stdout.flush()

    if i == 100:
	break

overall_now = time.time()
elapsed = overall_now - overall_then
mps = 100 / elapsed
print "mps:", mps


s.close()
sys.exit()
