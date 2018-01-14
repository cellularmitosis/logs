note: there are no timestamps in this data.  each data point ison a 0.25 second interval, and the temperature set-point increases by 1 degree Celsius every minute.

## run 1, kp: 2.0, ki: 1.0, kd: 0.5

first recorded run.  oops, I had forgot to turn the peltier fan on.

input:

![](run1-input.png)

output:

![](run1-output.png)

## run 2, kp: 2.0, ki: 1.0, kd: 0.5

I rearranged the connections to give AREF better filtering.

input:

![](run2-input.png)

output:

![](run2-output.png)

## run 3, kp: 2.0, ki: 2.0, kd: 0.5

increase ki from 1.0 to 2.0

input:

![](run3-input.png)

output:

![](run3-output.png)

## run 4, kp: 2.0, ki: 0.5, kd: 0.5

decreased ki from 2.0 to 0.5

input:

![](run4-input.png)

output:

![](run4-output.png)

## run 5, kp: 3.0, ki: 0.0, kd: 0.0

just using kp (3.0).

input:

![](run5-input.png)

output:

![](run5-output.png)

## run 6, kp: 4.0, ki: 0.0, kd: 0.0

bumping kp to 4.0.

input:

![](run6-input.png)

output:

![](run6-output.png)

## run 7, kp: 4.0, ki: 0.0, kd: 0.0

oops, accidentally repeated the same settings.  the repeatability is very good!

input:

![](run7-input.png)

output:

![](run7-output.png)

## run 8, kp: 8.0, ki: 0.0, kd: 0.0

bumping kp to 8.

input:

![](run8-input.png)

output:

![](run8-output.png)

## run 9, kp: 6.0, ki: 0.0, kd: 0.0

dropping kp to 6.

input:

![](run9-input.png)

output:

![](run9-output.png)

## run 10, kp: 5.0, ki: 0.0, kd: 0.0

dropping kp to 5.

input:

![](run10-input.png)

output:

![](run10-output.png)

## run 11, kp: 5.0, ki: 0.0, kd: 1.0

adding kd of 1.0.

input:

![](run11-input.png)

output:

![](run11-output.png)

## run 12, kp: 5.0, ki: 0.0, kd: 2.0

bumping kd to 2.0.

input:

![](run12-input.png)

output:

![](run12-output.png)

## run 13, kp: 8.0, ki: 0.0, kd: 1.0

dropping kd to 1.0, bumping kp to 8.0.

input:

![](run13-input.png)

output:

![](run13-output.png)

## run 14, kp: 6.0, ki: 0.0, kd: 1.0

dropping kp to 6.0.

input:

![](run14-input.png)

output:

![](run14-output.png)

## run 15, kp: 7.0, ki: 0.0, kd: 1.0

bumping kp to 7.0.

input:

![](run15-input.png)

output:

![](run15-output.png)

## run 16, kp: 7.0, ki: 0.5, kd: 1.0

adding ki of 0.5.

input:

![](run16-input.png)

output:

![](run16-output.png)

## run 17, kp: 7.0, ki: 0.5, kd: 1.0

Switching AREF from 3.3V to 5.0V.

I had implemented a heavy filter on the 3.3V line before it reached AREF and the thermistor: a large ferrite bead followed by a 1uF film cap to ground, then the 10k fixed resistor and 0.1uF film cap to ground, then the thermistor (to ground).

In each of the above graphs, you can see the stair-step at the end of the graph (in the cool-down phase).  This corresponds to individual LSB's of the ADC, and this can be a consquence of removing too much noise from the system.  I am implementing 64x oversampling in software, so with a bit of white noise, you should be able to achieve higher resolution from the ADC (for a slow-moving signal).

The 3.3V pin on the Arduino is already somewhat more filtered than the 5.0V pin, which is notorious for containing noise from the USB connection.  I am curious if switching to the 5.0V pin for AREF will give me enough noise to get better ADC resolution.

input:

![](run17-input.png)

output:

![](run17-output.png)

Looks like it worked exactly as expected!

## run 18, kp: 7.0, ki: 0.5, kd: 0.5

reducing kd to 0.5.  let's see if we can get the output noise down.

input:

![](run18-input.png)

output:

![](run18-output.png)

## run 19, kp: 7.0, ki: 0.5, kd: 0.75

bumping kd to 0.75.

input:

![](run19-input.png)

output:

![](run19-output.png)

## run 20, kp: 7.0, ki: 0.75, kd: 0.75

bumping ki to 0.75.

input:

![](run20-input.png)

output:

![](run20-output.png)
