#define PART_TM4C123GH6PM 1

#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include <inc/hw_types.h>
#include <inc/hw_memmap.h>
#include <inc/hw_gpio.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/qei.h>

#include "encoder.h"


typedef struct
{
    unsigned int base;
    bool invert;
} tEnc;

static tEnc _enc[2]=
{
  {QEI0_BASE, false},
  {QEI1_BASE, false}
};


void initEncoders()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI1);

    // Setup IO
    // Unlock PD7 (NMI)
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTD_BASE + GPIO_O_CR)  |= GPIO_PIN_7;
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;

    // QEI
    GPIOPinTypeQEI(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6);
    GPIOPinTypeQEI(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
    GPIOPinConfigure(GPIO_PD6_PHA0);
    GPIOPinConfigure(GPIO_PD7_PHB0);
    GPIOPinConfigure(GPIO_PC5_PHA1);
    GPIOPinConfigure(GPIO_PC6_PHB1);

   
    // Setup QEI Peripheral
    initQEI(0, false);
    initQEI(1, false);
}

void initQEI(unsigned char num, bool invert)
{
    if (num > 1)
        return;

    _enc[num].invert = invert;

    QEIDisable(QEI0_BASE);
    QEIConfigure(_enc[num].base, QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_NO_RESET | QEI_CONFIG_QUADRATURE | (_enc[num].invert ? QEI_CONFIG_SWAP : QEI_CONFIG_NO_SWAP), UINT_MAX);
    QEIPositionSet(_enc[num].base, UINT_MAX / 2);
    QEIEnable(QEI0_BASE);
}

void invertEncoder(unsigned char num)
{
    if (num > 1)
        return;

    initQEI(num, !(_enc[num].invert));
}


signed long readEnc(unsigned char num)
{
    if (num > 1)
        return 0;
  
    return QEIPositionGet(_enc[num].base) - UINT_MAX / 2;
}

