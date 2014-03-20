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

#include "led.h"
#include "servo.h"
#include "softservo.h"

#define delay(x)      SysCtlDelay(SysCtlClockGet() / 3 * x);

extern LED rLED, gLED, bLED;
extern Servo servo0,
             servo1,
             servo2,
             servo3,
             servo4,
             servo5;

extern SoftServo servo6,
                 servo7,
                 servo8,
                 servo9,
                 servo10,
                 servo11;


int main(void)
{
    // Clock (80MHz)
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
   
    
    // Initialize LEDs
    initLEDs();
    rLED.enable(true);
    gLED.enable(true);
    bLED.enable(true);

   /* 
    // Initialize IO Expander
    initPCA9557();


    // Initialize Motors
    initMotors();
    motor0.invert();
    motor0.setThrottle();
    motor1.invert();
    enableMotors();
    
    motor0.set();

    // Initialize Encoders
    initEncoders();
    enc0.invert();
    enableEncoders(ALL);

    */
    // Initialize Servos
    initServos();
    //servo0.setType();
    servo2.setLimits(0.2, 0.7);
    servo0.set(0.4);

    initSoftServos();
/*
    // Use ALL Pins for Servos
    initExtraServos();
    servo7.set();


    // Initialize IMU (MPU6050)
    initMPU6050();
    // ... Add more functions ...

    // Initialize Power Monitor (INA220)
    initINA220();
*/
    // Enable Interrupts
    IntMasterEnable();

    while(1)
    {
        //servo0 = ;
        //servo2 = ;
    }
}
