To evaluate the performance of my meter, I'll just log a VHP202Z for several hours.  My suspicion is that the "noise" issue I'm seeing is simply due to the fact that a 20k resistor only occupies the bottom 20% of the ADC's range, so when that signal gets scaled up in a graph, the resulting tempco looks very "noisy".

## run 1: 19k970 #3

testing VHP202Z 19K970 #3, soldered using very short 4-wire leads. 

connectors are [Cinch 108-0753-102](https://www.digikey.com/product-detail/en/cinch-connectivity-solutions-johnson/108-0753-102/J10108-ND/565813).  wire is 24 AWG cat 5, stripped.

![](IMG_2289.JPG)

![](IMG_2290.JPG)

![](run1-19k970-3/chart.png)

here we see the noisy result I was expecting.

## run 2: 9k9850 #1

we expect this result to be our best-case scenario, because it is near the top of the ADC's range.

![](run2-9k9850-1/chart.png)

## run 3: AE XB 25K000

this sits at 25% of the ADC's range, so we expect the noise to be slightly better than the 19K970.

![](run3-ae-xb-25k000/chart.png)
