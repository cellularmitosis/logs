#!/usr/bin/env gnuplot -c

set terminal png size 100,100 enhanced
set output 'output.png'

set lmargin 0
set rmargin 0
set tmargin 0
set bmargin 0
unset tics
unset border

set xdata time
set timefmt '%s'
set datafile separator ','
plot ARG1 using 1:2 notitle with points
