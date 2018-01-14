#!/usr/bin/env python3

# Note: you will need pyusb installed.
# Debian users: sudo apt-get install python3-usb

# You will need permissions on the UGSimple USB device.
# List the usb devices:
#   $ lsusb | grep Microchip 
#   Bus 004 Device 004: ID 04d8:000c Microchip Technology, Inc. 
# List the existing permissions for bus 004, device 004:
#   $ ls -l /dev/bus/usb/004/004
#   crw-rw-r-- 1 root root 189, 387 Aug 12 13:42 /dev/bus/usb/004/004
# Change the permissions:
#   $ sudo chmod ugo+rw /dev/bus/usb/004/004

from ugsimple.GPIB import UGSimpleGPIB

if __name__ == '__main__':
	# Requires root permissions (or add the udev rule)
	mygpib = UGSimpleGPIB()
	#mygpib = UGSimpleGPIB(debug_mode=True)

	mygpib.write( 17, "DABDHPaX" )
