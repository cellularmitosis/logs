#!/usr/bin/env gnuplot -c

set xdata time
set timefmt '%s'
set datafile separator ','
plot ARG1 using 1:2 notitle with points
pause mouse key
while (MOUSE_CHAR ne 'q') { pause mouse key; }
