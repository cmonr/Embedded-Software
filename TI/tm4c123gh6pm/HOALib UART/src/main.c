#include "uart.h"


#define delay(x)      SysCtlDelay(SysCtlClockGet() / 3 * x);


int main(void)
{
    // Clock (80MHz)
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // Init UART0
    UART_Init(uart[0]);
    // UART_SetBuad(115200);
    // UART_SetFormat();
    UART_Enable(uart[0]);

    //UART_IntConfig();
    //UART_IntEnable();
       
    while(1){
        unsigned char c = UART_ReadChar(uart[0]);
        //UART_ReadString(str);
        //UART_Read(buff, len);

        //printf("\r%c", c);
        UART_WriteChar(uart[0], c);
    }
}
