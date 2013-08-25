#include <stdbool.h>
#include <stdint.h>
#include <inc/hw_types.h>
#include <inc/hw_memmap.h>
#include <inc/hw_gpio.h>
#include <inc/hw_ints.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/interrupt.h>

#include "encoder.h"


typedef struct {
    int out[4];
} tFSMState;

static tFSMState FSM[4] = {
    {{ 0, 1,-1, 0}},
    {{-1, 0, 0, 1}},
    {{ 1, 0, 0,-1}},
    {{ 0,-1, 1, 0}}
};


static int freq[4] = {0, 0, 0, 0};

typedef struct{
    signed long value;
    tFSMState next;
} QEIEncoder;

static QEIEncoder _enc[2];

void initEncoders(bool inv0, bool inv1)
{
    // Encoders (Port D)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    //SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI0);

    // Unlock PD7
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTD_BASE + GPIO_O_CR) = GPIO_PIN_7;
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;

    GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
    GPIOIntTypeSet(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7, GPIO_BOTH_EDGES);
    GPIOIntEnable(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
  
    IntEnable(INT_GPIOD);


    //  Encoders (Port A)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_PIN_4 | GPIO_PIN_5, GPIO_BOTH_EDGES);
    GPIOIntEnable(GPIO_PORTA_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    IntEnable(INT_GPIOA);

    

    /*
    GPIOPinConfigure(GPIO_PD6_PHA0);
    GPIOPinConfigure(GPIO_PD7_PHB0);
    GPIOPinTypeQEI(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);

    QEIDisable(QEI0_BASE);
    QEIConfigure(QEI0_BASE, QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_NO_RESET | QEI_CONFIG_QUADRATURE | (inv0 ? QEI_CONFIG_SWAP : QEI_CONFIG_NO_SWAP), 1024);
    QEIPositionSet(QEI0_BASE, 512);
    QEIEnable(QEI0_BASE);

    // QEI1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI1);
    
    GPIOPinConfigure(GPIO_PC5_PHA0);
    GPIOPinConfigure(GPIO_PC6_PHB0);
    GPIOPinTypeQEI(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6);

    QEIDisable(QEI1_BASE);
    QEIConfigure(QEI1_BASE, QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_NO_RESET | QEI_CONFIG_QUADRATURE | (inv1 ? QEI_CONFIG_SWAP : QEI_CONFIG_NO_SWAP), 1024);
    QEIPositionSet(QEI1_BASE, 512);
    QEIEnable(QEI1_BASE);
    */
}

signed long readEnc(unsigned char num)
{
  if (num > 1)
    return 0;
  return _enc[num].value;
}


void PortAIntHandler(void)
{
    unsigned char ndx;
    
    // QEI State Machine
    ndx = GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4 | GPIO_PIN_5) >> 4;
    _enc[0].value += _enc[0].next.out[ndx];
    _enc[0].next = FSM[ndx];

    // Ack Interrupt
    GPIOIntClear(GPIO_PORTA_BASE, GPIO_PIN_4 | GPIO_PIN_5);
}


void PortDIntHandler(void)
{
    unsigned char ndx;
    
    // QEI State Machine
    ndx = GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7) >> 6;
    _enc[1].value += _enc[1].next.out[ndx];
    _enc[1].next = FSM[ndx];

    freq[ndx]++;

    // Ack Interrupt
    GPIOIntClear(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
}

