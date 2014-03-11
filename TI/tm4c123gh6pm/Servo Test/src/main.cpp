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

Led rLed,
    gLed,
    bLed;

Servo servo0,
      servo1,
      servo2,
      servo3,
      servo4,
      servo5;

int main(void)
{
    // Clock (80MHz)
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    
    // Configure LEDs
    LEDs = RGBLED();

    // Init Peripherals
    //   I2C0
    I2C0();

    //   PWM0 (Servos)
    PWM0(25000);

    //   PWM1 (Motors/LEDs)
    PWM1(50);

    //   QEI (Encoders)
    QEI0();
    QEI1();


    // Configure Devices
    //   RGB LEDs
    rLed.bind(PWM1, 5);
    gLed.bind(PWM1, 7);
    bLed.bind(PWM1, 6);

    //   Servos
    servo0.bind(PWM0, 0);
    servo1.bind(PWM0, 1);
    servo2.bind(PWM0, 2);
    servo3.bind(PWM0, 3);
    servo4.bind(PWM0, 4);
    servo5.bind(PWM0, 5);

    /*
    servo6.bind(RCTimer0, 0);
    servo7.bind(RCTimer0, 1);
    servo8.bind(RCTimer0, 2);
    servo9.bind(RCTimer0, 3);
    servo10.bind(RCTimer0, 4);
    servo11.bind(RCTimer0, 5);
    */
    
    servo0.setType();
    servo2.setLimits();

    servo2.enable();
    servo0.disable();


    //    Motors
    motor0.bind(PWM1, 3, GPIOF, 0);
    motor0.bind(PWM1, 2, GPIOF, 4);
    motor0.bind(PWM1, 1, GPIOC, 4);
    motor0.bind(PWM1, 0, GPIOC, 7);
    
    // Enable Interrupts
    IntMasterEnable();

    while(1)
    {
        //servo0 = ;
        //servo2 = ;
    }
}
