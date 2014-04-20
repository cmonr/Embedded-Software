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

// TODO: Unroll this in the future
#define UART0 &_uart[0]
#define UART1 &_uart[1]
#define UART2 &_uart[2]
#define UART3 &_uart[3]
#define UART4 &_uart[4]
#define UART5 &_uart[5]
#define UART6 &_uart[6]
#define UART7 &_uart[7]



typedef enum
{
    UART_TX_IRQ,
    UART_RX_IRQ,
    UART_IRQ_FLAGS
} tUART_IRQ_Flag;

extern unsigned long uart_int_flags[UART_IRQ_FLAGS];


typedef struct
{
    unsigned long periph;
    unsigned long base;

    tPinName tx_pin;
    tPinName rx_pin;
    unsigned int tx_pin_mux;
    unsigned int rx_pin_mux;

    unsigned int nvic_ndx;
    unsigned long irq_flags;
    void (*irq[UART_IRQ_FLAGS])( void );

    bool isInit;
} tUART;

extern tUART _uart[8];



void UART_Init( tUART* );

void UART_WriteChar( tUART*, unsigned char );
void UART_WriteStr( tUART*, unsigned char* );
void UART_Write( tUART*, unsigned char*, unsigned long );

unsigned char UART_ReadChar( tUART* );
unsigned int UART_ReadStr( tUART*, unsigned char* , unsigned long );
void UART_Read( tUART*, unsigned char*, unsigned long);

void UART_SetIRQ( tUART*, tUART_IRQ_Flag, void (*)() );
void UART_IntEnable( tUART*, tUART_IRQ_Flag );
void UART_IntDisable( tUART*, tUART_IRQ_Flag );

void UART_IRQ( tUART* );


void UART_Enable( tUART* );
void UART_Disable( tUART* );



void UART0IntHandler( void );
void UART1IntHandler( void );
void UART2IntHandler( void );
void UART3IntHandler( void );
void UART4IntHandler( void );
void UART5IntHandler( void );
void UART6IntHandler( void );
void UART7IntHandler( void );

extern tPin pins[PIN_COUNT];

#endif
