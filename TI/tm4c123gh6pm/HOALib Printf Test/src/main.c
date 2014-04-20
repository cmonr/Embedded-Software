#include <stdio.h>

#include "uart.h"

#define delay(x)      SysCtlDelay(SysCtlClockGet() * x);

#define rLED PF1
#define bLED PF2


void UART0_IRQ_TX()
{
    Pin_Toggle(rLED);
}


int main(void)
{
    // Clock (80MHz)
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // Init LEDs
    Pin_Init(rLED);
    Pin_Init(bLED);

    Pin_Set(rLED, LOW);
    Pin_Set(bLED, LOW);


    // Init UART0
    UART_Init(UART0);
    UART_SetIRQ(UART0, UART_TX_IRQ, &UART0_IRQ_TX);
    UART_IntEnable(UART0, UART_TX_IRQ);
    UART_Enable(UART0);

    // Enable NVIC
    IntMasterEnable();
       
    // Busy waiting since everything is interrupt driven ^_^
    while(1)
    {
        // Arbritrary statements  
        printf("Hi!\r\n");
        printf(" %0.2f + %0.2f = %0.2f\r\n", 0.2, 0.3, 0.2 + 0.3);

        delay(0.2);
    }
}

