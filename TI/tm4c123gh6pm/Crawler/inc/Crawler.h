#pragma once

#include "servo.h"
#include "softservo.h"

#define delay(x) SysCtlDelay(SysCtlClockGet() / 3 * (x));


class Crawler {
protected:
    Servo hservos[4];
    SoftServo servos[4];
    int NUM_SERVOS;
    float stepDelay;

public:
    Crawler();
    void crawlForward(void);
};
