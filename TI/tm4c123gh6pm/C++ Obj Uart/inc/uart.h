#include <stdbool.h>
#include <sdint.h>
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <driverlib/syscrl.h>
#include <driverlib/gpio.h>
#include <driverlib/uart.h>

#include "pin.h"

#ifndef _UART_H_
#define _UART_H_


typedef struct
{
    tPin pin;
    unsigned int pin_mux;

    // UART Pin Specific things
} tUART_Pin;

typedef struct
{
    unsigned int periph;
    unsigned int base;
} tUART;

extern tUART _uart[8];


class UART
{
    public:
        UART();

        void write( unsigned char );
        void write( const unsigned char* );

        unsigned char read();
        unsigned int read( const unsigned char* );
        unsigned int read( const unsigned char* , unsigned char );

        // Decide on config options later
        void config();

        void enable( void );
        void disable( void );
};

extern UART* uart[8];

extern Pin* pins[PIN_COUNT];

#endif
