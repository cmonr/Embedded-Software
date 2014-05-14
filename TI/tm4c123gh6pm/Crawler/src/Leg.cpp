#include "Leg.h"
  
Leg::Leg(Servo* knee, Servo* hip, int phase, bool smooth)
: hStepAdder(Stepper(phase, smooth)),
  kStepAdder(Stepper(phase, smooth)),
  knee(knee), 
  hip(hip)
{}

void Leg::step(void) {
    knee->set(kStepAdder.step());
    hip->set(hStepAdder.step());
}

Stepper::Stepper(int phase, bool smooth)
: numSteps(0), index(phase), smooth(smooth), initialized(false) 
{
    if (smooth) {
        index *= NUM_SUB_STEPS;
    }
}

Stepper& Stepper::operator<<(float step) {
    if (numSteps < MAX_STEPS) {
        steps[numSteps++] = step;
    }

    return *this;
}

void Stepper::init(void) {
    initialized = true;
    
    if (smooth) {
        substepsPerStep = NUM_SUB_STEPS / numSteps;

        for (int i = 0; i < numSteps - 1; i++) {
            steppers[i] = CubicStepper(steps[i], steps[i + 1], substepsPerStep);
        }

        steppers[numSteps - 1] = CubicStepper(
            steps[numSteps - 1], 
            steps[0], 
            NUM_SUB_STEPS
            );
    }
}

float Stepper::step(void) {
    if (!initialized) {
        init();
    }

    float res;

    if (smooth) {
        res = steppers[index / substepsPerStep].f(index % substepsPerStep); 
        index = (index + 1 ) % NUM_SUB_STEPS;
    } else {
        res = steps[index];
        index = (index + 1) % numSteps;
    }
    
    return res;
}

