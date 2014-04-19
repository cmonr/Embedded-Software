#include "uart.h"

#define delay(x)      SysCtlDelay(SysCtlClockGet() * x);

#define rLED PF1
#define gLED PF3

#define UART0 &_uart[0]


void UART0_IRQ_TX()
{
    Pin_Toggle(rLED);
}

void UART0_IRQ_RX()
{
    Pin_Set(gLED, HIGH);

    UART_WriteChar(UART0, '\r');
    UART_WriteChar(UART0, UART_ReadChar(UART0));

    Pin_Set(gLED, LOW);
}


int main(void)
{
    // Clock (80MHz)
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // Init LEDs
    Pin_Init(rLED);
    Pin_Init(gLED);

    Pin_Set(rLED, LOW);
    Pin_Set(gLED, LOW);


    // Init UART0
    UART_Init(UART0);

    UART_SetIRQ(UART0, UART_TX_IRQ, &UART0_IRQ_TX);
    //UART_SetIRQ(UART0, UART_RX_IRQ, &UART0_IRQ_RX);
    UART_IntEnable(UART0, UART_TX_IRQ);
    //UART_IntEnable(UART0, UART_RX_IRQ);

    UART_Enable(UART0);

    // Enable NVIC
    IntMasterEnable();
       
    while(1)
    {
        UART_WriteChar(UART0, UART_ReadChar(UART0));
    }
}

