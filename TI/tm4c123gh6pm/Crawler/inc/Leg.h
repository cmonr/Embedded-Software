#pragma once

#include "servo.h"
#include "CubicStepper.h"

#define MAX_STEPS 10
#define NUM_SUB_STEPS 20

class Stepper {
protected:
    float steps[MAX_STEPS];
    CubicStepper steppers[MAX_STEPS];
    int numSteps;
    int substepsPerStep;
    int index;
    bool smooth;
    bool initialized;

public:
    Stepper(int phase = 0, bool smooth = true);
    Stepper& operator<<(float step);
    float step(void);
    
protected:
    void init(void);
};

class Leg {
public:
    Stepper hStepAdder;
    Stepper kStepAdder;

protected:
    Servo* knee;
    Servo* hip;

public:
    Leg(Servo* knee, Servo* hip, int phase = 0, bool smooth = true);
    void step();
};

