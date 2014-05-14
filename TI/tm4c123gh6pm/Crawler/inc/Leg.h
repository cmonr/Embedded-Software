#pragma once

#include "softservo.h"
#include "servo.h"
#include "CubicStepper.h"

#define NUM_STEPS 4
#define NUM_SUB_STEPS 20

class Leg {
protected:
  GServo* knee;
  GServo* hip;

  int index;
  bool smooth;
  float ksteps[NUM_STEPS];
  float hsteps[NUM_STEPS];
  CubicStepper ksteppers[NUM_STEPS];
  CubicStepper hsteppers[NUM_STEPS];

public:
  Leg(GServo* knee, GServo* hip, int phase = 0, bool smooth = true)
  : knee(knee), 
    hip(hip), 
    smooth(smooth) 
  {
    index = phase;
    if (smooth) {
      index *= NUM_SUB_STEPS;
    }
  }
  
  void step();
  void setKSteps(float, float, float, float);
  void setHSteps(float, float, float, float);
};

