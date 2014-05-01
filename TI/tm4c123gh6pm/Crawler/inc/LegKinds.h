#pragma once

#include "softservo.h"
#include "Leg.h"

class LeftLeg : public Leg {
public:
    LeftLeg(SoftServo* knee, SoftServo* hip, int phase = 0)
    : Leg(knee, hip, phase) {
        ksteps[0] = 0.0;
        ksteps[1] = 0.5;
        ksteps[2] = 0.5;
        ksteps[3] = 0.0;
        
        hsteps[0] = 1.0;
        hsteps[1] = 1.0;
        hsteps[2] = 0.5;
        hsteps[3] = 0.5;
    }
};

class RightLeg : public Leg {
public:
    RightLeg(SoftServo* knee, SoftServo* hip, int phase = 0)
    : Leg(knee, hip, phase) {
        ksteps[0] = 0.0;
        ksteps[1] = 0.5;
        ksteps[2] = 0.5;
        ksteps[3] = 0.0;
        
        hsteps[0] = 1.0;
        hsteps[1] = 1.0;
        hsteps[2] = 0.5;
        hsteps[3] = 0.5;
    }
};

