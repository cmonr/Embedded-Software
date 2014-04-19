#include "uart.h"

#define delay(x)      SysCtlDelay(SysCtlClockGet() * x);

#define rLED PF1
#define bLED PF2

#define UART0 &_uart[0]


void UART0_IRQ_TX()
{
    Pin_Toggle(rLED);
}

void UART0_IRQ_RX()
{
    // Disable this for now. Really bright...
    //Pin_Toggle(bLED);

    UART_WriteChar(UART0, '\r');
    UART_WriteChar(UART0, UART_ReadChar(UART0));
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
    UART_SetIRQ(UART0, UART_RX_IRQ, &UART0_IRQ_RX);
    UART_IntEnable(UART0, UART_TX_IRQ);
    UART_IntEnable(UART0, UART_RX_IRQ);

    UART_Enable(UART0);

    // Enable NVIC
    IntMasterEnable();
       
    // Busy waiting since everything is interrupt driven ^_^
    while(1);
}

