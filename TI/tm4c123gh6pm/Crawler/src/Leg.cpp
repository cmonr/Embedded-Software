#include "Leg.h"
  
Leg::Leg(Servo* knee, Servo* hip, int phase, bool smooth)
: knee(knee), 
  hip(hip), 
  smooth(smooth),
  numKSteps(0),
  numHSteps(0),
  hStepAdder(StepAdder(hsteps, numHSteps)),
  kStepAdder(StepAdder(ksteps, numKSteps))
{
    index = phase;
    if (smooth) {
        index *= NUM_SUB_STEPS;
    }
}

void Leg::step(void) {
    float kval, hval;

    if (smooth) {
        kval = ksteppers[index / NUM_SUB_STEPS].f(index % NUM_SUB_STEPS);
        hval = hsteppers[index / NUM_SUB_STEPS].f(index % NUM_SUB_STEPS);
        index = (index + 1) % (MAX_STEPS * NUM_SUB_STEPS);
    } else {
        kval = ksteps[index];
        hval = hsteps[index];
        index = (index + 1) % MAX_STEPS;
    }
    
    knee->set(kval);
    hip->set(hval);
}

void Leg::setKSteps(float k1, float k2, float k3, float k4) {
    ksteps[0] = k1;
    ksteps[1] = k2;
    ksteps[2] = k3;
    ksteps[3] = k4;

    if (smooth) {
        for (int i = 0; i < MAX_STEPS - 1; i++) {
            ksteppers[i] = CubicStepper(ksteps[i], ksteps[i+1], NUM_SUB_STEPS);
        }

        ksteppers[MAX_STEPS-1] = CubicStepper(
            ksteps[MAX_STEPS-1], 
            ksteps[0], 
            NUM_SUB_STEPS
            );
    }
}

void Leg::setHSteps(float h1, float h2, float h3, float h4) {
    hsteps[0] = h1;
    hsteps[1] = h2;
    hsteps[2] = h3;
    hsteps[3] = h4;

    for (int i = 0; i < MAX_STEPS - 1; i++) {
        hsteppers[i] = CubicStepper(hsteps[i], hsteps[i+1], NUM_SUB_STEPS);
    }
    
    hsteppers[MAX_STEPS-1] = CubicStepper(
        hsteps[MAX_STEPS-1], 
        hsteps[0], 
        NUM_SUB_STEPS
        );
}

