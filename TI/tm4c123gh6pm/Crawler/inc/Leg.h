#pragma once

#include "servo.h"
#include "softservo.h"

#define NUM_STEPS 4

class Leg {
protected:
  SoftServo* knee;
  SoftServo* hip;
  float ksteps[NUM_STEPS];
  float hsteps[NUM_STEPS];

public:
  int index;
  Leg(SoftServo* knee, SoftServo* hip, int phase = 0)
  : knee(knee), hip(hip), index(phase) {}
  void step();
  void setKSteps(float, float, float, float);
  void setHSteps(float, float, float, float);
  void move(float, float);
};

