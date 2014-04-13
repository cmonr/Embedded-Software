#include <stdbool.h>
#include <stdint.h>
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/uart.h>
#include <driverlib/pin_map.h>
#include <driverlib/interrupt.h>

#include "pin.h"

#ifndef _UART_H_
#define _UART_H_

//#define UART_BUFF_SIZE 64

typedef struct
{
    unsigned int periph;
    unsigned int base;

    tPin tx_pin;
    tPin rx_pin;
    unsigned int tx_pin_mux;
    unsigned int rx_pin_mux;

    unsigned int int_ndx;
} tUART;

extern tUART _uart[8];


class UART
{
    public:
        typedef enum
        {
            FIVE  = UART_CONFIG_WLEN_5,
            SIX   = UART_CONFIG_WLEN_6,
            SEVEN = UART_CONFIG_WLEN_7,
            EIGHT = UART_CONFIG_WLEN_8
        } tDATA_LEN;

        typedef enum
        {
            ONE = UART_CONFIG_STOP_ONE,
            TWO = UART_CONFIG_STOP_TWO
        } tSTOP_BITS;

        typedef enum
        {
            NONE = UART_CONFIG_PAR_NONE,
            EVEN = UART_CONFIG_PAR_EVEN,
            ODD  = UART_CONFIG_PAR_ODD
        } tPARITY_BIT;

        typedef struct
        {
            tDATA_LEN length;
            tSTOP_BITS stop;
            tPARITY_BIT parity;
        } tFORMAT;

        typedef enum
        {
            TX,
            RX
        } tIRQ;

        UART( unsigned char );
        UART( unsigned char, unsigned long );
        UART( unsigned char, unsigned long, tDATA_LEN, tSTOP_BITS, tPARITY_BIT );

        void write( unsigned char );
        void writeBuff( unsigned char*, unsigned long );
        void writeStr( unsigned char* );

        unsigned char read( void );
		void readBuff( unsigned char*, unsigned long);
        unsigned int readStr( unsigned char* , unsigned long );


        void enable( void );
        void disable( void );

        void setIRQHandler( tIRQ, void (*)(void) );
		void enableIRQ( void );
		void disableIRQ( void );

		void IRQ( void );

    private:
        unsigned char _ndx;
        tFORMAT _format;

        unsigned long _irq_mask;
        void (*_tx_irq)(void);
        void (*_rx_irq)(void);
};

extern UART* uart[8];

extern Pin* pins[PIN_COUNT];

extern "C" {
void UART0IntHandler( void );
void UART1IntHandler( void );
void UART2IntHandler( void );
void UART3IntHandler( void );
void UART4IntHandler( void );
void UART5IntHandler( void );
void UART6IntHandler( void );
void UART7IntHandler( void );
}

#endif
