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

    // I2C0
    I2C_Init(I2C0);
    I2C_Enable(I2C0);
    

    // Init Objects
    PCA9557 pca9557 = PCA9557(I2C0);
    pca9557.set(P2, LOW);
    pca9557.set(P3, LOW);


    while(1)
    {
        Pin_Toggle(PF1);

        pca9557.toggle(P2);

        delay(0.2);
    }
}

