#include "Leg.h"

void Leg::step(void) {
    knee->set(ksteps[index]);
    hip->set(hsteps[index]);
    index = (index+1)%NUM_STEPS;
}

void Leg::setKSteps(float k1, float k2, float k3, float k4) {
    ksteps[0] = k1;
    ksteps[1] = k2;
    ksteps[2] = k3;
    ksteps[3] = k4;
}

void Leg::setHSteps(float h1, float h2, float h3, float h4) {
    hsteps[0] = h1;
    hsteps[1] = h2;
    hsteps[2] = h3;
    hsteps[3] = h4;
}

