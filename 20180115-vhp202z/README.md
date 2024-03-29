logging some Vishay VHD202Z hermetically sealed Z-foil resistors.

![](IMG_2260.JPG)

## run 1: 9K9850 #1

10 steps of 1C, 5 minutes per step.

![](run1-9k9850-1/IMG_2261.JPG)

![](run1-9k9850-1/tempco.png)

-1.3ppm/K?  Are you kidding me?  This is disappointing to say the least.  So much for 0.2ppm/C.  http://www.vishaypg.com/docs/63120/hzseries.pdf

## run 2: 9K9850 #1, with maximum possible lead length

Let's see what the effect of lead length is on the tempco.

![](run2-9k9850-1/IMG_2262.JPG)

![](run2-9k9850-1/tempco.png)

no apparent difference.

## run 3: 9K9850 #2

![](run3-9k9850-2/IMG_2263.JPG)

![](run3-9k9850-2/tempco.png)

Bummer, this one is just as bad: -1.35ppm/K.

Looks like I made a mistake and truncated the lead-in data.  The chart starts at the jump from 25C to 26C.

## run 4: 9K9850 #3

testing the third 9K9850.  this was a quick run (2 minutes per C).  I want to get through the rest of these resistors quickly to see if they are all ~1.3ppm/K.

![](run4-9k9850-3/IMG_2266.JPG)

![](run4-9k9850-3/tempco.png)

Looks like -0.65ppm/K.

## run 5: 9K9850 #3

repeating the 9K9850 resistor #3 but at 5min/C.

![](run5-9k9850-3/tempco.png)

## run 6: 19K970 #1

I also have three VHP202Z 19K970 resistors.  Here's the first.  5min/C.

![](run6-19k970-1/tempco.png)

whoa, yuck, that is terribly noisy.  perhaps a bad solder joint?


## run 7: 19K970 #2

5min/C.


![](run7-19k970-2/tempco.png)

ugh, another grabage result.  where is all of this noise coming from all of a sudden?


## run 8: 19K970 #2

I unplugged a laptop adapter which was on the same circuit as my HP34401A (perhaps there is an EMI problem on the 120V line?).  Repeating the same run.  5min/C.

oh wait, I just realized I jumped to a different resistance range when I moved from 10k to 20k resistors.  I'm now at the bottom of the next range, using only 20% of the ADC's range.


![](run8-19k970-2/tempco.png)

## run 9: 19K970 #2

just repeating the same run again.

![](run9-19k970-2/tempco.png)
