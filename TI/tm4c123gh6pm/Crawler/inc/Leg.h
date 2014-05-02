#pragma once

#include "softservo.h"
#include "servo.h"

#define NUM_STEPS 4

class Leg {
protected:
  SoftServo* knee;
  SoftServo* hip;

public:
  Servo* hknee;
  Servo* hhip;
  float ksteps[NUM_STEPS];
  float hsteps[NUM_STEPS];
  int index;
  Leg(SoftServo* knee, SoftServo* hip, int phase = 0)
  : knee(knee), 
    hip(hip), 
    hknee(0), 
    hhip(0), 
    index(phase) {}
  
  Leg(Servo* hknee, Servo* hhip, int phase = 0)
  : knee(0), 
    hip(0), 
    hknee(hknee),
    hhip(hhip), 
    index(phase) {}
  
  void step();
  void setKSteps(float, float, float, float);
  void setHSteps(float, float, float, float);
  void move(float, float);
};

