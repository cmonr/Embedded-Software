#include "Leg.h"

void Leg::step(void) {
    float kval, hval;

    if (smooth) {
        kval = ksteppers[index / NUM_SUB_STEPS].f(index % NUM_SUB_STEPS);
        hval = hsteppers[index / NUM_SUB_STEPS].f(index % NUM_SUB_STEPS);
        index = (index + 1) % (NUM_STEPS * NUM_SUB_STEPS);
    } else {
        kval = ksteps[index];
        hval = hsteps[index];
        index = (index + 1) % NUM_STEPS;
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
        for (int i = 0; i < NUM_STEPS - 1; i++) {
            ksteppers[i] = CubicStepper(ksteps[i], ksteps[i+1], NUM_SUB_STEPS);
        }

        ksteppers[NUM_STEPS-1] = CubicStepper(
            ksteps[NUM_STEPS-1], 
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

    for (int i = 0; i < NUM_STEPS - 1; i++) {
        hsteppers[i] = CubicStepper(hsteps[i], hsteps[i+1], NUM_SUB_STEPS);
    }
    
    hsteppers[NUM_STEPS-1] = CubicStepper(
        hsteps[NUM_STEPS-1], 
        hsteps[0], 
        NUM_SUB_STEPS
        );
}

