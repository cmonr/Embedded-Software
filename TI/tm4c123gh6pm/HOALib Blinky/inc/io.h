#include <stdbool.h>
#include <stdint.h>
#include <inc/hw_memmap.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>

#ifndef _PIN_H_
#define _PIN_H_

#define PORT_WIDTH 8
#define PORT_COUNT PIN_COUNT/PORT_WIDTH


typedef enum {
    PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7,
    PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7,
    PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7,
    PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7,
    PE0, PE1, PE2, PE3, PE4, PE5, PE6, PE7,
    PF0, PF1, PF2, PF3, PF4, PF5, PF6, PF7,
    PIN_COUNT, ERR
} tPin;

typedef struct
{
    unsigned int periph;
    unsigned int base;
} tPort;

extern tPort _ports[6];


typedef enum { HiZ, LOW, HIGH } tIOPin_State;

tPin IO_Init( tPin );
void IO_Set( tPin, tIOPin_State );
void IO_Toggle( tPin );
unsigned char IO_Read( tPin );

void IO_Enable( tPin );
void IO_Disable( tPin );


typedef struct{
    bool isInit;

    tPort port;
    unsigned char offset;

    tIOPin_State state;
} tIOPin;

extern tIOPin pins[PIN_COUNT];

#endif
