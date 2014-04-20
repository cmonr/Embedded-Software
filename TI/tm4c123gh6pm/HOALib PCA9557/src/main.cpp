extern "C" {
#include "pin.h"
}

#include "pca9557.h"


#define delay(x)      SysCtlDelay(SysCtlClockGet() * x);


int main(void)
{
    // Clock (80MHz)
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);


    // Init peripherals
    //  Init Red LED Pin
    Pin_Init(PF1);
    Pin_Set(PF1, LOW);



    // Init Objects
    PCA9557 pca9557 = PCA9557(I2C0);
    pca9557.set(P0, OUT);
    pca9557.read(P0);
_

    while(1)
    {
        Pin_Toggle(PF1);

        pca9557.toggle(P0);

        delay(0.1);
    }
}

