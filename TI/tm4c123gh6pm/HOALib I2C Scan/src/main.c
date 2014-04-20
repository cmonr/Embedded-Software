#include <stdio.h>

#include "uart.h"
#include "i2c.h"

#define delay(x)      SysCtlDelay(SysCtlClockGet() * x);

#define rLED PF1

int i;

int main(void)
{
    // Clock (80MHz)
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // Init LEDs
    Pin_Init(rLED);
    Pin_Set(rLED, LOW);

    // Init UART0
    UART_Init(UART0);
    UART_Enable(UART0);
    setbuf(stdout, NULL);   // Disable printf internal buffer

    // Init I2C0
    I2C_Init(I2C0);
    I2C_Enable(I2C0);



    // Wait until user presses enter
    UART_ReadChar(UART0);

    // Scan for I2C addresses
    for(i=0; i < (1 << 7); i++)
    {
        printf("x%02x:", i);
        if (I2C_Write(I2C0, i, 0) == true)
            printf("* ");
        else    
            printf("  ");

        Pin_Toggle(rLED);


        if (i % 8 == 7)
            printf("\r\n");
    }

    // Indicator LED off
    Pin_Set(rLED, LOW);
       
    while(1);      
}

