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
#include <driverlib/pwm.h>
#include <driverlib/pin_map.h>
#include <driverlib/interrupt.h>
#include <driverlib/sysctl.h>


//#include "roboticsBP.h"
#include "pin.h"

#define delay(x)      SysCtlDelay(SysCtlClockGet() / 3 * x);


int main(void)
{
    // Clock (80MHz)
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
   

    // Initialize Robotics BoosterPack
    //RoboticsBP roboticsBP = RoboticsBP();

    Pin led = Pin(PF1);
    led.setDir(OUT);

    while(1){
        led.toggle();
        delay(1);
    }
}
