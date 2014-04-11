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
        pwm[1] -> set(5, 0.25);
        delay(1);

        pwm[1] -> set(5, 0.5);
        delay(1);

        pwm[1] -> set(5, 0.75);
        delay(1);
    }
}
