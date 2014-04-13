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

#define UART_BUFF_SIZE 64

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


typedef struct
{

} tUART_Mode;

class UART
{
    public:
        UART( unsigned char, unsigned long, tUART_Mode );

        void write( unsigned char );
        void write( const unsigned char* );

        unsigned char read();
        unsigned int read( const unsigned char* , unsigned char );
		unsigned int readUntil(const unsigned char*, unsigned char);

        // Decide on config options later
        void config();

        void enable( void );
        void disable( void );

		void enableIRQ( void );
		void disableIRQ( void );

		void defaultIRQ( void );
};

extern UART* _uart[8];

extern Pin* pins[PIN_COUNT];

#endif
