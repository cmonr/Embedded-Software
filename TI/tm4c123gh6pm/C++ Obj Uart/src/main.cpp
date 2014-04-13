//#include <stdarg.h>
//#include <string.h>
#include "uart.h"

#define delay(x)      SysCtlDelay(SysCtlClockGet() / 3 * x);

int main(void)
{
    // Clock (80MHz)
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // Init UART0
    UART(0, 115200);
    uart[0] -> enable();


    //uart[0] -> 
    /*UART0.write();
    //UART0.read();
    UART0.config();
    UART0.enable();
    UART0.disable();*/


    // Setup LED PWM Periph
    /*pwm[1] -> enable(5);

    while(1){
        pwm[1] -> set(5, 0.2);
        delay(1);

        pwm[1] -> set(5, 0.0);
        delay(1);
    }*/
    
    while(1){
        //uart[0] -> write('H');
        delay(1);

        //uart[0] -> write('i');
        delay(0.5);

        //uart[0] -> write('\r');
        //uart[0] -> write('\n');

        delay(0.25);
    }
}
