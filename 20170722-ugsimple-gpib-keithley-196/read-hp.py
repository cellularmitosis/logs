#!/usr/bin/env python3

from ugsimple.GPIB import UGSimpleGPIB

if __name__ == '__main__':
    addr = 23
    gpib = UGSimpleGPIB(debug_mode=True)
    print(gpib.read(23))

