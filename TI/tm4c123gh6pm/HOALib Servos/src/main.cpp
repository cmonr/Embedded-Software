extern "C" {
#include <stdio.h>

#include "pin.h"
#include "pwm.h"
#include "uart.h"
}

#include "servo.h"

#define delay(x)      SysCtlDelay(SysCtlClockGet() * x);


int main(void)
{
    // Clock (80MHz)
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);


    // Init peripherals
    //  Init Red LED Pin
    Pin_Init(PF1);
    Pin_Set(PF1, LOW);

    //  Init PWM0/1
    PWM_Init(PWM0, 50);
    //PWM_Init(PWM1, 1000);

    // PWM gets enabled by objects using PWM channels

    //  Init UART0
    UART_Init(UART0);
    UART_Enable(UART0);
    setbuf(stdout, NULL);   // Disable printf internal buffer



    // Init Objects
    Servo s0 = Servo(PWM0, 0);
    Servo s1 = Servo(PWM0, 1);
    Servo s2 = Servo(PWM0, 2);
    Servo s3 = Servo(PWM0, 3);
    Servo s4 = Servo(PWM0, 4);
    Servo s5 = Servo(PWM0, 5);


    // Wait until user presses enter
    UART_ReadChar(UART0);

    // Indicator LED on
    Pin_Set(PF1, HIGH);

    // Enable Servos
    s0.enable();
    s1.enable();
    s2.enable();
    s3.enable();
    s4.enable();
    s5.enable();

    delay(1);

    // Dance forever!
    while(1)
    {
        Pin_Toggle(PF1);

        delay(0.1);

        s0.set(0.6);
        s1.set(0.6);
        s2.set(0.6);
        s3.set(0.6);
        s4.set(0.6);
        s5.set(0.6);
        
        Pin_Toggle(PF1);

        delay(0.1);

        s0.set(0.4);
        s1.set(0.4);
        s2.set(0.4);
        s3.set(0.4);
        s4.set(0.4);
        s5.set(0.4);
    }
}

