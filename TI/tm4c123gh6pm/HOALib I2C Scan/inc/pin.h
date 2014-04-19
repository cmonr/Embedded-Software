#include <stdbool.h>
#include <stdint.h>
#include <inc/hw_memmap.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>

#ifndef _PIN_H_
#define _PIN_H_

#define PORT_WIDTH 8
#define PORT_COUNT PIN_COUNT/PORT_WIDTH


typedef struct
{
    unsigned int periph;
    unsigned int base;
} tPort;

extern tPort ports[6];


typedef enum {
    PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7,
    PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7,
    PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7,
    PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7,
    PE0, PE1, PE2, PE3, PE4, PE5, PE6, PE7,
    PF0, PF1, PF2, PF3, PF4, PF5, PF6, PF7,
    PIN_COUNT, ERR, NONE
} tPinName;

typedef enum { HiZ, LOW, HIGH } tPin_State;

typedef struct{
    bool isInit;

    tPort port;
    unsigned char offset;

    tPin_State state;
} tPin;

extern tPin pins[PIN_COUNT];


tPinName Pin_Init( tPinName );
void Pin_Set( tPinName, tPin_State );
void Pin_Toggle( tPinName );
unsigned char Pin_Read( tPinName );

void Pin_Enable( tPinName );
void Pin_Disable( tPinName );

#endif
