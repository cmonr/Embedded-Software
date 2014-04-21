#ifndef _UART_H_
#define _UART_H_

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
#include "io-classes.h"

//#define UART_BUFF_SIZE 64

typedef struct
{
    unsigned long periph;
    unsigned long base;

    tPin tx_pin;
    tPin rx_pin;
    unsigned int tx_pin_mux;
    unsigned int rx_pin_mux;

    unsigned int int_ndx;

    unsigned int baud;

    bool isInit;
} tUART;

class UART : public CharDevice {
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


        typedef enum
        {
            TX,
            RX
        } tIRQ;

        UART( unsigned char );
        UART( unsigned char, unsigned long );
        UART( unsigned char, unsigned long, tDATA_LEN, tSTOP_BITS, tPARITY_BIT );
  virtual int putc ( int );
  virtual int getc ();
  virtual int open ();
  virtual int close ();
  virtual int seek ( long int offset, int origin );

        void setIRQHandler( tIRQ, void (*)(void) );
	void enableIRQ( void );
	void disableIRQ( void );

	void IRQ( void );

    private:
        typedef struct
        {
unsigned long baud;
            tDATA_LEN length;
            tSTOP_BITS stop;
            tPARITY_BIT parity;
        } tFORMAT;

        int _ndx;
        tFORMAT _format;

        unsigned long _irq_mask;
        void (*_tx_irq)(void);
        void (*_rx_irq)(void);
};


void UART0IntHandler( void );
void UART1IntHandler( void );
void UART2IntHandler( void );
void UART3IntHandler( void );
void UART4IntHandler( void );
void UART5IntHandler( void );
void UART6IntHandler( void );
void UART7IntHandler( void );

#endif
