#pragma once

#include "servo.h"
#include "CubicStepper.h"

#define MAX_STEPS 10
#define NUM_SUB_STEPS 20

class StepAdder {
protected:
    float* steps;
    int& numSteps;

public:
    StepAdder(float* steps, int& numSteps) : steps(steps), numSteps(numSteps) {}
    StepAdder& operator<<(float step) {
        if (numSteps < MAX_STEPS) {
            steps[numSteps++] = step;
        }

        return *this;
    }
};

class Leg {
protected:
    Servo* knee;
    Servo* hip;
    bool smooth;
    int index;
    int numKSteps;
    int numHSteps;
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

