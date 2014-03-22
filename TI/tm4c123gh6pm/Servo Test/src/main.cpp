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

#include "led.h"
#include "servo.h"
//#include "softservo.h"

#define delay(x)      SysCtlDelay(SysCtlClockGet() / 3 * x);

/*
extern SoftServo servo6,
                 servo7,
                 servo8,
                 servo9,
                 servo10,
                 servo11;
*/

int main(void)
{
    // Clock (80MHz)
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
   
    
    // Initialize LEDs
    initLEDs();
    
    LED rLED = LED(PWM_OUT_5, PWM_OUT_5_BIT);
    rLED.enable();


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

    Servo servo0 = Servo(PWM_OUT_0, PWM_OUT_0_BIT);
    Servo servo1 = Servo(PWM_OUT_1, PWM_OUT_1_BIT);
    Servo servo2 = Servo(PWM_OUT_2, PWM_OUT_2_BIT);
    Servo servo3 = Servo(PWM_OUT_3, PWM_OUT_3_BIT);
    Servo servo4 = Servo(PWM_OUT_4, PWM_OUT_4_BIT);
    Servo servo5 = Servo(PWM_OUT_5, PWM_OUT_5_BIT);

    // Cascade initialize
    //  Limit sudden power draw
    servo0.enable();
    delay(0.25);
    servo1.enable();
    delay(0.25);
    servo2.enable();
    delay(0.25);
    servo3.enable();
    delay(0.25);
    servo4.enable();
    delay(0.25);
    servo5.enable();
    delay(0.25);

    //initSoftServos();
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
        delay(1);
        rLED.set(1.0);
        servo0.set(0.6);
        servo1.set(0.6);
        servo2.set(0.6);
        servo3.set(0.6);
        servo4.set(0.6);
        servo5.set(0.6);

        delay(1);
        rLED.set(0);
        servo0.set(0.4);
        servo1.set(0.4);
        servo2.set(0.4);
        servo3.set(0.4);
        servo4.set(0.4);
        servo5.set(0.4);
    }
}
