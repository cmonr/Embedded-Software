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

extern "C" {
#include "i2c.h"
}

#include "led.h"
#include "pca9557.h"

#define delay(x)      SysCtlDelay(SysCtlClockGet() / 3 * x);


int main(void)
{
    // Clock (80MHz)
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
   
    
    // Initialize LEDs
    initLEDs();
    
    LED rLED = LED(PWM_OUT_5, PWM_OUT_5_BIT);
    rLED.enable();


    // Initialize I2C0
    I2CInit();

    // Initialize IO Expander
    PCA9557 pca9557 = PCA9557();
    pca9557.setDir(2, pca9557.OUT); // Servo Enable

/*
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
        delay(0.25);
        rLED.set(1.0);
        pca9557.write(2, pca9557.HIGH);
       
        delay(0.25);
        rLED.set(0);
        pca9557.write(2, pca9557.LOW);
    }
}
