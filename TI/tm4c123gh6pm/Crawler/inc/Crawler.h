#pragma once

#include "hardservo.h"
#include "softservo.h"

#define delay(x) SysCtlDelay(SysCtlClockGet() / 3 * (x));

class Crawler {
protected:
    HardServo hservos[4];
    SoftServo servos[4];
    int NUM_SERVOS;
    float stepDelay;

public:
    Crawler();
    void crawlForward(void);
};
