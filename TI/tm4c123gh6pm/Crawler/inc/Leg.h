#pragma once

#include "servo.h"
#include "softservo.h"

#define NUM_STEPS 4

class Leg {
protected:
  SoftServo* knee;
  SoftServo* hip;
  int index;
  int ksteps[NUM_STEPS];
  int hsteps[NUM_STEPS];

public:
  Leg(SoftServo* knee, SoftServo* hip, int phase = 0)
  : knee(knee), hip(hip), index(phase) {}
  void step(void);

protected:
  void setKSteps(float, float, float, float);
  void setHSteps(float, float, float, float);
};

