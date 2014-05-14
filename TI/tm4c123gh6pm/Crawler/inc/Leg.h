#pragma once

#include "servo.h"
#include "CubicStepper.h"

#define MAX_STEPS 10

class Stepper {
protected:
    float steps[MAX_STEPS];
    CubicStepper steppers[MAX_STEPS];
    int numSteps;
    int numSubSteps;
    int substepsPerStep;
    int index;
    bool smooth;
    bool initialized;

public:
    Stepper(int phase = 0, bool smooth = true, int numSubSteps = 80);
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
    Leg(Servo* knee, Servo* hip, int phase = 0, bool smooth = true, int numSubSteps = 80);
    void step();
};

