#include "io.h"

#define delay(x)      SysCtlDelay(SysCtlClockGet() / 3 * x);

tPin led;

int main(void)
{
    // Clock (80MHz)
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // Init Red Led
    led = IO_Init(PF1);

    IO_Enable(led);

    while(1){
        IO_Toggle(led);  
        delay(0.25);
    }
}
