## run0 

this was a run using the Arduino PID Autotune library.

results:
- p: 5.75
- i: 0.01

code excerpt from a modified copy of the example program which comes with the library:

```
byte ATuneModeRemember = 2;
double input = 0, output = 0, setpoint = 400;
double kp = 1, ki = 0.1, kd = 0;

double outputStart = 0;
//double aTuneStep = 127, aTuneNoise = 100, aTuneStartValue = 127;
double aTuneStep = 127, aTuneNoise = 5, aTuneStartValue = 127;
unsigned int aTuneLookBack = 300;

boolean tuning = true;
unsigned long  modelTime, serialTime;

PID myPID(&input, &output, &setpoint,kp,ki,kd, DIRECT);
PID_ATune aTune(&input, &output);

void setup()
{
```

the example program was also modified to spit out CSV and to terminate (infinite loop) upon completion of the tuning run:

```
  if (millis()>serialTime) {
    if (tuning) {
      Serial.print(setpoint);
      Serial.print(",");
      Serial.print(input);
      Serial.print(",");
      Serial.println(output);
    } else {
      Serial.print("kp: ");Serial.print(myPID.GetKp());Serial.print(" ");
      Serial.print("ki: ");Serial.print(myPID.GetKi());Serial.print(" ");
      Serial.print("kd: ");Serial.print(myPID.GetKd());Serial.println();
      while (true) delay(1);
    }
```

hmm, looking at the output chart, I don't think this worked correctly.  I think I set the `aTuneNoise` too low.

## run1

this was a test run to check on the above results.

- p: 5.75
- i: 0.01
- period: 300ms
- set point: 400

looks like `ki` is too small.

![](1517039205-run1/si7021.png)

![](1517039205-run1/adc.png)

## run2

another autotune run.

```
double input = 0, output = 0, setpoint = 400;
double kp = 3, ki = 0.1, kd = 0;

double outputStart = 0;
double aTuneStep = 127;
double aTuneNoise = 50;
double aTuneStartValue = 127;
unsigned int aTuneLookBack = 300;
```

![](1517041068-run2/chart.png)

hmm, the results seem to be junk (p=1.26, i=0).

## run3

- p: 10
- i: 0.025
- period: 300ms
- set point: 405

![](1517076098-run3/si7021.png)

![](1517076098-run3/adc.png)

![](1517076098-run3/output.png)

looks like I don't have enough Ki to steer the final correction.

