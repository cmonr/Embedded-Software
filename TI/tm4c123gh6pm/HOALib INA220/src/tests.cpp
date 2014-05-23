#include "roboticsbp.h"
#include "tests.h"


void wait(){ SysCtlDelay(SysCtlClockGet() * 0.1); }

void ledTest(tPWM* pwm, unsigned char pin)
{
    float i;
    for(i=0; i<0.2; i+=0.01)
    {
        PWM_Set(pwm, pin, i);
        wait();
    }

    for(; i>0; i-=0.01)
    {
        PWM_Set(pwm, pin, i);
        wait();
    }
}

void motorTest(DRV8800* motor)
{
    (*motor).set(0.7); 
    wait();
    (*motor).set(0.0); 
    wait();
    (*motor).set(-0.7); 
    wait();
    (*motor).set(0.0); 
    wait();
}

void servoTest(Servo* servo)
{
    (*servo).enable();
    wait();
    (*servo).set(0.35);
    wait();
    (*servo).set(0.65);
    wait();
    (*servo).disable();
    wait();
}

