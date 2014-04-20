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

extern tUART uart[8];

/*
        typedef enum
        {
            FIVE  = UART_CONFIG_WLEN_5,
            SIX   = UART_CONFIG_WLEN_6,
            SEVEN = UART_CONFIG_WLEN_7,
            EIGHT = UART_CONFIG_WLEN_8
        } tUART_DATA_LEN;

        typedef enum
        {
            ONE = UART_CONFIG_STOP_ONE,
            TWO = UART_CONFIG_STOP_TWO
        } tUART_STOP_BITS;

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
*/

void UART_Init( tUART );

void UART_WriteChar( tUART, unsigned char );
void UART_WriteStr( tUART, unsigned char* );
void UART_Write( tUART, unsigned char*, unsigned long );

unsigned char UART_ReadChar( tUART );
unsigned int UART_ReadStr( tUART, unsigned char* , unsigned long );
void UART_Read( tUART, unsigned char*, unsigned long);



void UART_Enable( tUART );
void UART_Disable( tUART );

//void setIRQHandler( tIRQ, void (*)(void) );
//void enableIRQ( void );
//void disableIRQ( void );

//void IRQ( void );



extern tIOPin pins[PIN_COUNT];

void UART0IntHandler( void );
void UART1IntHandler( void );
void UART2IntHandler( void );
void UART3IntHandler( void );
void UART4IntHandler( void );
void UART5IntHandler( void );
void UART6IntHandler( void );
void UART7IntHandler( void );

#endif
