#define PART_TM4C123GH6PM 1

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


#include "servo.h"


#define toggleRed()    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, ~GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1))
#define toggleBlue()   GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, ~GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2))
#define toggleGreen()  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, ~GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_3))

#define delay(x)      SysCtlDelay(SysCtlClockGet() / 3 * x);


void init(void)
{
    // Clock (80MHz)
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    
    
    // GPIO (LEDs)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);


    // Servos
    // first configure, then enable
    // invertServo()
    // setServoLimits()
    // setServoType()   // For pre-determined limits
    enableServos();


    // Enable Interrupts
    IntMasterEnable();
}

float i;

int main(void)
{ 
    init();

    enableServo(3);

/*
    // Forever Sawtooth
    while(1)
    {
        // Up
        for(i=-1.0; i<=1.0; i+=0.01)
        {
            setServo(0, i);
            setServo(1, i);

            delay(0.1);
        }
       
        toggleRed();

        // Down
        for(i=1.0; i>=-1.0; i-=0.01)
        {
            setMotor(0, i);
            setMotor(1, i);

            delay(0.1);
        }

        toggleRed();
    }*/

    while(1);
}
