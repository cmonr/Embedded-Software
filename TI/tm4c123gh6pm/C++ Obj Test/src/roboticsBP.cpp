#include "pwm.h"

#include "roboticsBP.h"



RoboticsBP::RoboticsBP()
{
    // Initialize Peripherals
    pwm[1] = new PWM(1, 25000);


    // Initialize BP Objects
    //   LEDs
    rLED = LED(pwm[1], 5);
    gLED = LED(pwm[1], 7);
    bLED = LED(pwm[1], 6);
}
