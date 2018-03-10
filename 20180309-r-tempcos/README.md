more 4-wire mode with DUT in the TEC chamber, ramping from 25C to 35C, etc.

- meter: 34401A in 4-wire mode.
- chamber: Hammond 1590A with 12706 peltier, Arduino-based half-bridge controller.
- power supply set to 5V.
- Si7021 records ambient temperature.

## run 1

GenRad 1434-P 2k resistance card #1, resistor #1.

- 1C/minute, with 6 minute plateaus at 25C and 35C.

This was just a quick run to verify my setup and to see if the tempco was negative.

![](run1/chart.png)

