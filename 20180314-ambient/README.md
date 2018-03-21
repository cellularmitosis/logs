ambient temperature logs (looking for the most stable spot in my apartment).

## run 1

the bedroom, near my lamp.

![](run1-1521100465/chart.png)


## run 2

overnight in a cardboard box with a small appliance light bulb as heat source, then heat source turned off during the day while I was at work.

![](run2-1521179085/chart.png)


## run 3

same cardboard box, but with an inlet and outlet tube added (toilet paper cardboard tubes),
and a small 12V fan affixed to a tube in an exhaust configuration.

The fan was initially off, then turned on as 12V, then lowered to 4.5V (roughly the lowest starting voltage).


![](run3-1521264354/chart.png)

## run 4

Same box, same fan, but with a PID loop controlling the duty cycle of the fan.  Even with only spending 15 minutes tuning the PID, and with the relatively noisy / coarse readins of the Si7021, it was easy to get +/-0.1C.

The first chart is the first 5000 data points (roughly the first half hour), and the second chart represents over 12 hours of data.


![](run4-1521470249/chart.png)


![](run4-1521470249/chart2.png)


## run 5

a short test run with the larger box, to establish a low-temperature baseline.


## run 6

an overnight test run with the meter turned on, but the fan turned off (to see the temp rise).

![](run6-1521530629/chart.png)

## run 7

PID set to 24C.

couldn't quite make that.

![](run7-1521553757/chart.png)


## run 8

PID set to 24.5C.

![](run8-1521556808/chart.png)

## run 9

PID set to 25C.
