#include <stdio.h>

#include "pin.h"

#include "pca9557.h"
#include "drv8800.h"
#include "uart.h"


#define delay(x)      SysCtlDelay(SysCtlClockGet() * x);

#define rLED  PWM1_5 

int main(void)
{
    // Clock (80MHz)
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);


    // Init peripherals
    //  UART0
    UART_Init(UART0);
    UART_Enable(UART0);
    setbuf(stdout, NULL);

    //  I2C0
    I2C_Init(I2C0);
    I2C_Enable(I2C0);

    //  PWM1
    PWM_Init(PWM1, 25000);
    PWM_Enable(rLED);
    PWM_Set(rLED, 0.0);
    

    // Init Objects
    PCA9557 pca9557 = PCA9557(I2C0);
    pca9557.set(P3, LOW);   // Motors start disabled

    DRV8800 m0 = DRV8800(PWM1_3, PF0);
    DRV8800 m1 = DRV8800(PWM1_2, PF4);
    DRV8800 m2 = DRV8800(PWM1_1, PC4);
    DRV8800 m3 = DRV8800(PWM1_0, PC7);

    // Enable motors
    pca9557.set(P3, HIGH);
    printf("\r\n\n");

    while(1)
    {
        float i;
        for(i=0.0; i<1.0; i+=0.01)
        {
            delay(0.01);
            PWM_Set(rLED, i * 0.2);
            m0.set(i);
            m1.set(i);
            m2.set(i);
            m3.set(i);

            printf("\r% 0.2f ", i);
        }

        printf("\r\n");
        
        for(i=1.0; i>-1.0; i-=0.01)
        {
            delay(0.01);
            PWM_Set(rLED, i * 0.2);
            m0.set(i);
            m1.set(i);
            m2.set(i);
            m3.set(i);

            printf("\r %0.2f ", i);
        }
        
        for(i=-1.0; i<0.0; i+=0.01)
        {
            delay(0.01);
            PWM_Set(rLED, i * 0.2);
            m0.set(i);
            m1.set(i);
            m2.set(i);
            m3.set(i);

            printf("\r %0.2f ", i);
        }
    }
}

