#include "pwm.h"

#define delay(x)      SysCtlDelay(SysCtlClockGet() / 3 * x);

int main(void)
{
    // Clock (80MHz)
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // Init PWM1
    PWM(1, 25000);

    // Setup Red Led
    pwm[1] -> enable(5);

    while(1){
        float i=0;
        for(; i<0.2; i+=0.001)
        {
            pwm[1] -> set(5, i);
            delay(0.001);
        }

        for(; i>=0.0; i-=0.001)
        {
            pwm[1] -> set(5, i);
            delay(0.001);
        }
    }
}
