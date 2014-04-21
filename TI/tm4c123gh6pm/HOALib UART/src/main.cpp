#include "uart.h"


#define delay(x)      SysCtlDelay(SysCtlClockGet() / 3 * x);


int main(void)
{
    // Clock (80MHz)
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    UART testio = UART(0);

    // Init UART0
    // UART_SetBuad(115200);
    // UART_SetFormat();

    //UART_IntConfig();
    //UART_IntEnable();
       
    while(!testio.putc(testio.getc()));
    while(1);
}
