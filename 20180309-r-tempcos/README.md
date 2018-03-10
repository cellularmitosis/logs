more 4-wire mode with DUT in the TEC chamber, ramping from 25C to 35C, etc.

- meter: 34401A in 4-wire mode, slow 6-digit.
- chamber: Hammond 1590A with 12706 peltier, Arduino-based half-bridge controller.
- power supply set to 5V.
- Si7021 records ambient temperature.

## run 1

GenRad 1434-P **2k** resistance **card #1, resistor #1**.

- 1 minute/C, with 6 minute plateaus at 25C and 35C.

This was just a quick run to verify my setup and to see if the tempco was negative.

![](run1/chart.png)

hmm, looks like I've got a bit of instability in my oven's PID loop:

![](run1/oven.png)

## run 2

GenRad 1434-P **2k** resistance **card #1, resistor #1**.

- 1 minute/C, with 24-minute plateaus at 25C and 35C.

![](run2/chart.png)

About -5.8ppm/C.

## run 3

GenRad 1434-P **2k** resistance **card #1, resistor #2**.

- 5 minute/C, with 30-minute plateaus at 25C and 35C.

![](run3/chart.png)

About -6.3ppm/C.

## run 4

GenRad 1434-P **2k** resistance **card #1, resistor #3**.

- 30-minute plateau at 35C.

![](run4/chart.png)

About -5.6ppm/C.

## run 5

GenRad 1434-P **2k** resistance **card #1, resistor #3**, with hand-made copper wire resistor for tempco compensation.

- copper wire resistor is 31 inches of 40AWG magnet wire wrapped around a 10meg resistor, in series with the 2k resistor.
- 30-minute plateau at 35C.

![](run5/chart.png)

![](run5/chart-zoomed.png)

As expected, there is some thermal lag between the two resistors, but after the temperature stabilizes, the tempco appears to be beneath the drift of my measurement system (probably 0.5ppm/C).  Pretty neat!

