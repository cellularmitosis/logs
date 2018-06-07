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


(note: the following runs have been re-ordered according to PID parameters)

## run 16

* kp 4000
* ki 0
* kd 0

![](run16-1522212234/chart.png)


## run 15

* kp 2000
* ki 0
* kd 0

![](run15-1522201679/chart.png)


## run 14

* kp 1000
* ki 0
* kd 0

![](run14-1522194978/chart.png)


## run 17

* kp 500
* ki 0
* kd 0

![](run17-1522280986/chart1.png)

![](run17-1522280986/chart2.png)

![](run17-1522280986/chart3.png)


## run 4

Same box, same fan, but with a PID loop controlling the duty cycle of the fan.  Even with only spending 15 minutes tuning the PID, and with the relatively noisy / coarse readins of the Si7021, it was easy to get +/-0.1C.

The first chart is the first 5000 data points (roughly the first half hour), and the second chart represents over 12 hours of data.

edit: not 100% sure what the PID constants were.  according to git they were likely:
- kp: 500
- ki: 30
- kd: 0

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

edit: not 100% sure what the PID constants were.  according to git they were likely:
- kp: 500
- ki: 30
- kd: 0


![](run7-1521553757/chart.png)


## run 8

PID set to 24.5C.

edit: not 100% sure what the PID constants were.  according to git they were likely:
- kp: 500
- ki: 30
- kd: 0

![](run8-1521556808/chart.png)


## run 9

edit: not 100% sure what the PID constants were.  according to git they were likely:
- kp: 500
- ki: 30
- kd: 0

![](run9-1521587227/chart.png) 


## run 10

added another Si7021 to sense ambient temp outside of the box.

PID set to 25C.

still had two instances of the fan speed running away.  not sure what is causing that.  at the same time, the values from the outside temp sensor are a perfectly flat line.  hrmm...

![](run10-1522111226/chart.png)


## run 11

- kp: 250
- ki: 30
- kd: 0

![](run11-1522128944/chart.png)

## run 12

switching to a mosfet-based linear fan control.  pwm drives a low-pass filter to the gate of a 2N7000, which is in series with the fan.

* kp 200
* ki 10
* kd 0

internal Si7021 is 9x oversampled, external sampled once per loop, which is about a 2 second loop (takes about 202ms to read from the Si7021).

![](run12-1522136535/chart1.png)
![](run12-1522136535/chart2.png)


## run 13

* kp 200
* ki 5
* kd 0

![](run13-1522161596/chart1.png)

![](run13-1522161596/chart2.png)


## run 18

* kp 125
* ki 1
* kd 0

![](run18-1522419216/chart1.png)

![](run18-1522419216/chart2.png)

![](run18-1522419216/chart3.png)


## run 19

* kp 500
* ki 2
* kd 0

![](run19-1522460862/chart1.png)

![](run19-1522460862/chart2.png)

![](run19-1522460862/chart3.png)

## run 20

* kp 125
* ki 1
* kd 0

changed the sampling period: 14 internal samples and 1 external sample per loop (roughly a 3 second loop).


## run 21

* kp 125
* ki 4
* kd 0
* set-point: 25C


## run 22

from the previous runs, it looks like this small fan can't keep up at the temperature high-point of each cycle.
I'll raise the set-point by a half degree and see if that helps.

note: the previous runs were at 25C.

* kp 125
* ki 4
* kd 0
* set-point: 25.5C

## run 23

messing around with the minimum fan speed (I noticed it stalled on 90).  setting it to minimum of 94.

* kp 125
* ki 4
* kd 0
* set-point: 25.5C


## run 24

* kp 125
* ki 4
* kd 0
* set-point: 25.1C
* min fan speed 93

