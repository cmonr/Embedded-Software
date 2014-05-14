#pragma once

#include "hardservo.h"
#include "softservo.h"

#define delay(x) SysCtlDelay(SysCtlClockGet() / 3 * (x));

class Crawler {
protected:
    HardServo hservos[4];
    SoftServo sservos[4];
    int NUM_SERVOS;
    float stepDelay;

public:
    Crawler(float stepDelay = .015*20.0);
    void crawlForward(void);
};
