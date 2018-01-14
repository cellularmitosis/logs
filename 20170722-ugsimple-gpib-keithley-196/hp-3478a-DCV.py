#!/usr/bin/env python3

from ugsimple.GPIB import UGSimpleGPIB

if __name__ == '__main__':
    addr = 23
    gpib = UGSimpleGPIB()

    cmd = ""
    cmd += "F1" # DC volts
    cmd += "RA" # auto-range
    cmd += "Z1" # auto-zero
    cmd += "T3" # single trigger

    gpib.write(addr, cmd)
    print(gpib.read(addr))

