#!/usr/bin/env python

import sys
import serial
import time

def create_serials():
    serials = []
    for arg in sys.argv[1:]:
        # 9600, 8N1
        ser = serial.Serial(
            port = arg, # e.g. /dev/ttyACM0 or /dev/ttyUSB0
            baudrate = 9600,
            bytesize = serial.EIGHTBITS,
            stopbits = serial.STOPBITS_ONE,
            parity = serial.PARITY_NONE,
            timeout = 0
        )
        serials.append(ser)
    return serials

# Adapted from https://learn.adafruit.com/thermistor/circuitpython
def steinhart_temperature_C(r, Ro=10000.0, To=25.0 - 0.735, beta=3997.0):
    import math
    r = float(r)
    steinhart = math.log(r / Ro) / beta      # log(R/Ro) / beta
    steinhart += 1.0 / (To + 273.15)         # log(R/Ro) / beta + 1/To
    steinhart = (1.0 / steinhart) - 273.15   # Invert, convert to C
    return str(steinhart)

def line_proc(line):
    values = line.split(',')
    try:
        hp_34401a_thermistor = float(values[1])
        mcp9701_c = float(values[6])
    except:
        # this is the header line.  replace the timestamp and HP value with a header.
        values[0] = 'timestamp'
        values[1] = '34401a_thermistor'
        values.append('hp_error_c')
        values.append('hp_mcp_delta')
        line = ','.join(values)
        return line
    hp_error = hp_34401a_thermistor - 25.0
    values.append(str(hp_error))
    delta = hp_34401a_thermistor - mcp9701_c
    values.append(str(delta))
    line = ','.join(values)
    return line

if __name__ == "__main__":
    serials = create_serials()

    buffers = []
    for ser in serials:
        buffers.append("")

    last_lines = []
    for ser in serials:
        last_lines.append(None)
    
    last_line_times = []
    for ser in serials:
        last_line_times.append(time.time())

    processors = []
    for ser in serials:
        processors.append(None)

    line_processor = line_proc

    processors[0] = steinhart_temperature_C

    while True:
        for i in range(len(serials)):
            ser = serials[i]
            while True:
                ch = ser.read(1)
                if len(ch):
                    if ch == '\n' or ch == '\r':
                        line = buffers[i]
                        buffers[i] = ""
                        then = last_line_times[i]
                        now = time.time()
                        last_line_times[i] = now
                        if len(line) and now - then >= 0.1:
                            if processors[i]:
                                line = processors[i](line)
                            last_lines[i] = line
                    else:
                        buffers[i] += ch
                else:
                    break

        should_print = True
        for i in range(len(serials)):
            if last_lines[i] == None:
                should_print = False

        if should_print:
            values = []
            values.append(str(time.time()))
            for line in last_lines:
                for value in line.split(','):
                    values.append(value)
            output = ','.join(values)
            if line_processor:
                output = line_processor(output)
            sys.stdout.write(output + '\n')
            sys.stdout.flush()
            last_lines[0] = None

        time.sleep(0.01)
