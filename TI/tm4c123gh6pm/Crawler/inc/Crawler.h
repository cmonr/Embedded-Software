#pragma once

#include "softservo.h"

#define delay(x) SysCtlDelay(SysCtlClockGet() / 3 * (x));


class Crawler {
protected:
    SoftServo servos[4];
    int NUM_SERVOS;
    float stepDelay;

public:
    Crawler();
    void crawlForward(void);
};
