#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <driverlib/gpio.h>
#include <driverlib/uart.h>
#include <driverlib/i2c.h>
#include <driverlib/pin_map.h>
#include <driverlib/interrupt.h>
#include <driverlib/sysctl.h>

#include "rgbled.h"

#define delay(x)      SysCtlDelay(SysCtlClockGet() / 3 * x);

RGBLED LEDs;

int main(void)
{
    // Clock (80MHz)
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    
    // Configure LEDs
    LEDs = RGBLED();
    
    // Enable Interrupts
    IntMasterEnable();

    while(1)
    {
        LEDs.set(1.0, 0, 0);
        delay(1);

        LEDs.set(0, 1.0, 0);
        delay(1);

        LEDs.set(0, 0, 1.0);
        delay(1);
    }

}
