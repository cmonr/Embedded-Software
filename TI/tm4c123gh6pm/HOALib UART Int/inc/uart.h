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


typedef enum
{
    UART_TX_IRQ = 0,
    UART_RX_IRQ = 1,
    UART_IRQ_FLAGS = 2
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
    void (*irq[2])( void ); // Two IRQ Flags. #define this somewhere later...

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
