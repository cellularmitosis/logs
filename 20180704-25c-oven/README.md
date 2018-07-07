## run 1

uninsulated oven prototype, no compensation.

soldered a 20k in parallel with the 10k below the thermistor, to ajust the set-point to a higher temperature.

measuring the difference between the inverting and non-inverting op amp inputs, using a Keithley 2015.

## run 2

removing 20k parallel resistor.

measuring the difference between the inverting and non-inverting op amp inputs, using a Keithley 2015.


## run 3

New setup: I discovered I had mistakenly thought that pin 1 was the base of the current limiting transistor,
when in fact this is pin 2.  This is why my circuit wasn't behaving.

I have constructed another Hammond 1590B case, with foam insulation, which simply has an MJE3055 and two
10k thermistors epoxied to the bottom (using Arctic Silver thermal epoxy), with all of the control
circuitry off-board on a breadboard.

I am now logging the ambient temperature (using an Si7021) and the temperature of one of the affixed thermistors (using the "TEMP" function of a Keithley 2015).

Initial log will simply be logging the off-state behavior of the device.

## run 4

turned on the power to the LM723 circuit.

hmm, it turned on and just railed to the current-limited output (140mA) the entire time, shooting way past the set-point.

## run 6

attempted to reverse which position the thermistor was in (lower half of non-inverting divider, rather than upper half of inverting divider), but ran into the same problems (the circuit keeps heating past the set-point).
