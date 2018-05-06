#!/bin/bash

./trivial-serial.py /dev/tty.usbmodem1421 | tee -a log.csv
