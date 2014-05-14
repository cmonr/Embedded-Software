#pragma once

#include "servo.h"
#include "CubicStepper.h"

#define MAX_STEPS 10
#define NUM_SUB_STEPS 20

class StepAdder {
protected:
    float* steps;
    int index;

public:
    StepAdder() : steps(0), index(-1) {}
    StepAdder(float* steps) : steps(steps), index(0) {}
    StepAdder& operator<<(float step) {
        if (index < MAX_STEPS) {
            steps[index++] = step;
        }

        return *this;
    }
};

class Leg {
protected:
    Servo* knee;
    Servo* hip;
    int index;
    bool smooth;
    float ksteps[MAX_STEPS];
    float hsteps[MAX_STEPS];
    CubicStepper ksteppers[MAX_STEPS];
    CubicStepper hsteppers[MAX_STEPS];

public:
    Leg(Servo* knee, Servo* hip, int phase = 0, bool smooth = true);
    void step();
    void setKSteps(float, float, float, float);
    void setHSteps(float, float, float, float);
    StepAdder hStepAdder;
    StepAdder kStepAdder;
};

