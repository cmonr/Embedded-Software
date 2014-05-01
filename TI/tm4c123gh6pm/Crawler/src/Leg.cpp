#include "Leg.h"

void Leg::step(void) {
    knee->set(ksteps[index]);
    hip->set(hsteps[index]);
    index = (index+1)%NUM_STEPS;
}

