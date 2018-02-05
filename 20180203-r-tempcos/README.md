## run 1

This was a run with the AE XT 10K000 which I had previously measured, just to see if this would work at all.

While setting this up I accidentally drove the temperature of the chamber up very high (12V, 3A into the peltier, with the hot side going into the chamber, for about 10 minutes).  When I noticed it was drawing current, I shut off the power supply.  A few minutes later I checked the log output and it was coming down, currently at 50C.  I had to prey the lid off because the hot glue had re-melted and adhered to the lid.  Oops.

Anyway, it looks like this first graph is garbage.  Probably a lot of hysteresis.

## run 2, 3, 4

repeating this measurement to observe this hysteresis phenomenon.

## run 5

switched from the voltage divider to a true constant-current supply.

circuit:

- LT6657-1.25 to non-inverting pin of LTC2057
- LTC2057 output to Rdut
- Rdut to LTC2057 inverting input and Rshunt
- Rshunt to ground

12.5k was chosen as the shunt, which puts 1V across Rdut (10k).  Rshunt is made up of a 10k and 2.5k metal foil resistors.

the circuit is powered by a 2-cell lipo battery, using one cell for the LT6657 and two cells for the LTC2057.

initial setup before datalogging shows that this setup is prone to noise pickup.  I can move the reading around by dozens of microvolts just by adjusting where I stand.  that's enough to totally swamp my reading.

well, the result looks like garbage.  oh, perhaps I need a capacitor across the feedback resistor.  derp.

## run 6

attempted to add a few more capacitors to filter EMI.  one across the shunt, one across the DUT, and one across the wires just before they enter the meter.


