#pragma once

#include "servo.h"
#include "CubicStepper.h"

#define NUM_STEPS 4
#define NUM_SUB_STEPS 20

class Leg {
protected:
    Servo* knee;
    Servo* hip;

    int index;
    bool smooth;
    float ksteps[NUM_STEPS];
    float hsteps[NUM_STEPS];
    CubicStepper ksteppers[NUM_STEPS];
    CubicStepper hsteppers[NUM_STEPS];

public:
    Leg(Servo* knee, Servo* hip, int phase = 0, bool smooth = true);
    void step();
    void setKSteps(float, float, float, float);
    void setHSteps(float, float, float, float);
};

