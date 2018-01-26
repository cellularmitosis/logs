This was a project to develop a thermal chamber which can maintain a constant temperature environment (of 25C).

## Enclosure

The chamber is a Hammond 1590BB diecast aluminum case.  Twelve 10-Ohm resistors have been affixed to the underside with thermal epoxy.  The chamber has been insulated with 1/4-inch "craft foam".

![](IMG_2325.jpg)

## Initial proof-of-concept board

This was a very rough "first draft" to see if the circuit would work at all.

![](IMG_2322.jpg)

![](IMG_2324.jpg)

It was pretty noisy, but the concept appeared sound.  I decided to implement a second draft on an Arduino "prototype" shield:

![](IMG_2328.jpg)

![](IMG_2326.jpg)

![](IMG_2327.jpg)

![](IMG_2329.jpg)

![](IMG_2331.jpg)

An Si7021 was used as the DUT to verify the temperature tracking.

![](IMG_2330.jpg)

## run0:

These were a few initial runs as I was getting the setup connected and working.

During one of these runs I noticed a problem where the temperature measurement showed a siginicant offset when the 12V power supply (which powers the heater resistors) was turned on or off:

![](1516861366-run0/shot1.png)

## run2:

- p: 0.5, i: 0.1, d: 0.01
- set: 450
- period: 300ms

![](1516863539-run2/shot1.png)


## run3:

- p: 0.75, i: 0.075, d: 0
- set: 400
- period: 300ms

the unit left regulation overnight, when the temperature in my apartment dropped (went to bed at 71F, woke up at 9am at 67F).

![](1516867509-run3/shot1.png)

![](1516867509-run3/shot2.png)

## run4:

- p: 0.75, i: 0.075, d: 0
- set: 400
- period: 300ms

this run was just to demonstrate the problem of the 12V supply influencing the temperature reading.

half way into this run I turned off the 12V supply for a minute, then turned it back on.  total run was about 3 minutes.

![](1516893458-run4/shot1.png)

