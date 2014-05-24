#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "pin.h"
#include "pwm.h"
#include "uart.h"

#include "roboticsbp.h"
#include "tests.h"

#define rLED  PWM1_5 
#define gLED  PWM1_7
#define bLED  PWM1_6


#define CMD_DELIM ';'

DRV8800* motors[4] = {&m0, &m1, &m2, &m3};
Servo* servos[6] = {&s0, &s1, &s2, &s3, &s4, &s5};


int main(void)
{
    // Clock (80MHz)
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);


    // Init peripherals
    //  UART0/1
    UART_Init(UART0);
    //UART_SetIRQ(UART0, UART_RX_IRQ, &UART0_RX_IRQ);
    //UART_IntEnable(UART0, UART_RX_IRQ);
    UART_Enable(UART0);

    //UART_Init(UART1);
    //UART_SetIRQ(UART1, UART_RX_IRQ, &UART1_RX_IRQ);
    //UART_IntEnable(UART1, UART_RX_IRQ);
    //UART_Enable(UART1);
    setbuf(stdout, NULL);

    //  I2C0
    I2C_Init(I2C0);
    I2C_Enable(I2C0);

    //  PWM0/1
    PWM_Init(PWM0, 50);
    PWM_Init(PWM1, 1000);
    PWM_Enable(rLED);
    PWM_Enable(gLED);
    PWM_Enable(bLED);
    PWM_Set(rLED, 0);
    PWM_Set(gLED, 0);
    PWM_Set(bLED, 0);
    

    // Init Robotics BoosterPack
    RoboticsBP_Init();

    // NVIC
    IntMasterEnable();

    /*
    // Power On Self Test
    //printf("POST Started");

    //  LEDs
    ledTest(rLED);    
    ledTest(gLED);    
    ledTest(bLED);    

    //  Motors
    //   First enable motors
    pca9557.set(P3, HIGH);
    motorTest(&m0);
    motorTest(&m1);
    motorTest(&m2);
    motorTest(&m3);
    pca9557.set(P3, LOW);

    //  Servos
    servoTest(&s0);
    servoTest(&s1);
    servoTest(&s2);
    servoTest(&s3);
    servoTest(&s4);
    servoTest(&s5);

    //printf("\rPOST Completed\r\n\n");*/

    while(1)
    {
        unsigned short voltage = ina220.readVoltage();
        printf("% .3fv\r", voltage * 0.004);

        SysCtlDelay(SysCtlClockGet()/3/10);
    }
}

