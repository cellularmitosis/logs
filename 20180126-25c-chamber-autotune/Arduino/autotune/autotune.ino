#include "PID_v1.h"
#include "PID_AutoTune_v0.h"

double input = 0, output = 0, setpoint = 400;
double kp = 3, ki = 0.1, kd = 0;

double outputStart = 0;
double aTuneStep = 127;
double aTuneNoise = 50;
double aTuneStartValue = 127;
unsigned int aTuneLookBack = 300;

byte ATuneModeRemember = 2;

boolean tuning = true;
unsigned long serialTime;

PID myPID(&input, &output, &setpoint,kp,ki,kd, DIRECT);
PID_ATune aTune(&input, &output);

void setup()
{
  analogReference(EXTERNAL);

  pinMode(6, OUTPUT);
  pinMode(A0, INPUT);

  myPID.SetMode(AUTOMATIC);

  if (tuning) {
    tuning=false;
    changeAutoTune();
    tuning=true;
  }
  
  serialTime = 0;
  Serial.begin(9600);

  delay(250);
  Serial.println("setpoint,input,ouput");
}

void loop() {
  uint32_t accumulator = 0;
  uint16_t samples = 256;
  for (uint16_t i = 0; i < samples; i++) {
    accumulator += analogRead(A0);  
  }
  input = double(accumulator) / samples;
  
  if(tuning) {
    byte val = (aTune.Runtime());
    if (val != 0) {
      tuning = false;
    }
    if(!tuning) { //we're done, set the tuning parameters
      kp = aTune.GetKp();
      ki = aTune.GetKi();
      kd = aTune.GetKd();
      myPID.SetTunings(kp,ki,kd);
      AutoTuneHelper(false);
    }
  }
  else {
    myPID.Compute();
  }
  
  analogWrite(6,output); 
  
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
    
    serialTime+=500;
  }
}

void changeAutoTune() {
 if (!tuning) {
    //Set the output to the desired starting frequency.
    output = aTuneStartValue;
    aTune.SetNoiseBand(aTuneNoise);
    aTune.SetOutputStep(aTuneStep);
    aTune.SetLookbackSec((int)aTuneLookBack);
    AutoTuneHelper(true);
    tuning = true;
  } else { //cancel autotune
    aTune.Cancel();
    tuning = false;
    AutoTuneHelper(false);
  }
}

void AutoTuneHelper(boolean start) {
  if(start)
    ATuneModeRemember = myPID.GetMode();
  else
    myPID.SetMode(ATuneModeRemember);
}

