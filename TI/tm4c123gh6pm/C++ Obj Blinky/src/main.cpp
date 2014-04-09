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
    led.setDir(OUTPUT);

    while(1){
        led.toggle();
        delay(1);
    }


/*
    // Initialize Peripherals
    //   PWM0/1
    PWM0 = PWM(0, 50);
    PWM1 = PWM(1, 25000);

    
    // Initialize Robotics BoosterPack Objects
    //   LEDs
    LED rLED = LED(PWM1_6);
    LED gLED = LED(PWM1_7);
    LED bLED = LED(PWM1_5);
    rLED.enable();
    
    //   IO Expander
    PCA9557 pca9557 = PCA9557(I2C0);
    //PCA9557 pca9557 = PCA9557(I2C0, 1);
    pca9557.setDir(3, pca9557.OUT);
    pca9557.write(3, pca9557.HIGH);

    //   Motors
    DRV8800 motor0 = DRV8800(PWM1_0, PF0);
    DRV8800 motor1 = DRV8800(PWM1_1, PF4);
    DRV8800 motor2 = DRV8800(PWM1_2, PC4);
    DRV8800 motor3 = DRV8800(PWM1_3, PC7);

    //   Encoders
    enc0 = QEI0;
    enc1 = QEI1;
    
    //   Servos
    Servo servo0 = Servo(PWM0_0);
    Servo servo1 = Servo(PWM0_1);
    Servo servo2 = Servo(PWM0_2);
    Servo servo3 = Servo(PWM0_3);
    Servo servo4 = Servo(PWM0_4);
    Servo servo5 = Servo(PWM0_5);

    //   SoftServo & Controller
    SoftServoController softServoController = SoftServoController(WTIMER5);
    SoftServo servo6 = SoftServo(softServoController, PD2);
    SoftServo servo7 = SoftServo(softServoController, PD3);
    SoftServo servo8 = SoftServo(softServoController, PE0);
    SoftServo servo9 = SoftServo(softServoController, PE1);
    SoftServo servo10 = SoftServo(softServoController, PE2);
    SoftServo servo11 = SoftServo(softServoController, PE3);


    // Do thins after all objects are initialized
    //motor0.invert();
    //motor0.throttle(0.8);
    //motor1.invert();

    //enableMotors();
    
    //motor0.set();
    // Initialize Encoders
    enc0 = QEI0;
    enc0.invert();
    enc0.enable();


    // Initialize IMU (MPU6050)
    MPU6050 imu = MPU6050(I2C0, 0);
    // ... Add more functions ...

    // Initialize Power Monitor (INA220)
    INA220 monitor = INA220(I2C0);
    monitor.readVoltage();
    monitor.readCurrent();

    */

    // Enable Interrupts
    //IntMasterEnable();


    //while(1);
}
