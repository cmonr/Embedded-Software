#include "uart.h"


tUART _uart[8] = 
{
    {SYSCTL_PERIPH_UART0, UART0_BASE, PA1, PA0, GPIO_PA1_U0TX, GPIO_PA0_U0RX, INT_UART0},
    {SYSCTL_PERIPH_UART1, UART1_BASE, PB1, PB0, GPIO_PB1_U1TX, GPIO_PB0_U1RX, INT_UART1},
    {SYSCTL_PERIPH_UART2, UART2_BASE, PD7, PD6, GPIO_PD7_U2TX, GPIO_PD6_U2RX, INT_UART2},
    {SYSCTL_PERIPH_UART3, UART3_BASE, PC7, PC6, GPIO_PC7_U3TX, GPIO_PC6_U3RX, INT_UART3},
    {SYSCTL_PERIPH_UART4, UART4_BASE, PC5, PC4, GPIO_PC5_U4TX, GPIO_PC4_U4RX, INT_UART4},
    {SYSCTL_PERIPH_UART5, UART5_BASE, PE5, PE4, GPIO_PE5_U5TX, GPIO_PE4_U5RX, INT_UART5},
    {SYSCTL_PERIPH_UART6, UART6_BASE, PD5, PD4, GPIO_PD5_U6TX, GPIO_PD4_U6RX, INT_UART6},
    {SYSCTL_PERIPH_UART7, UART7_BASE, PE1, PE0, GPIO_PE1_U7TX, GPIO_PE0_U7RX, INT_UART7},
};

UART::UART(unsigned char ndx)
{
    // TODO: Add check for previous allocation
    //   For now, assume user is competent
    uart[ndx] = this;

    // Local variables
    _baud = 115200;
    _format = {EIGHT, ONE, NONE};


    // Power UART Peripheral
    SysCtlPeripheralEnable(_uart[ndx].periph);

    // Configure UART
    UARTConfigSetExpClk(_uart[ndx].base, _baud, _format.length | _format.stop | _format.parity);
}

UART::UART(unsigned char ndx, unsigned long baud)
{
    // TODO: Add check for previous allocation
    //   For now, assume user is competent
    uart[ndx] = this;

    // Local variables
    _baud = baud;
    _format = {EIGHT, ONE, NONE};


    // Power UART Peripheral
    SysCtlPeripheralEnable(_uart[ndx].periph);

    // Configure UART
    UARTConfigSetExpClk(_uart[ndx].base, _baud, _format.length | _format.stop | _format.parity);

}

UART::UART(unsigned char ndx, unsigned long baud, tUART_BIT_LEN len, tUART_STOP_BITS stop, tUART_PARITY_BIT parity)
{
    // TODO: Add check for previous allocation
    //   For now, assume user is competent
    uart[ndx] = this;

    // Local variables
    _baud = baud;
    _format = {len, stop, parity};


    // Power UART Peripheral
    SysCtlPeripheralEnable(_uart[ndx].periph);

    // Configure UART
    UARTConfigSetExpClk(_uart[ndx].base, _baud, _format.length | _format.stop | _format.parity);

}

void UART::enable()
{
    // Too much dereferencing. Let's minimize that a bit
    tPWM_Pin tx_pin = _uart[_ndx].tx_pin;
    tPWM_Pin rx_pin = _uart[_ndx].rx_pin;

    // Enable Pin Port
    // TODO: Initialize this better?
    Pin(tx_pin);
    Pin(rx_pin);
    
    // Set GPIO Pin Mux
    GPIOPinTypeUART(tx_pin -> port.base, tx_pin -> offset);
    GPIOPinTypeUART(rx_pin -> port.base, rx_pin -> offset);
    GPIOPinConfigure(_uart[_ndx].tx_pin_mux);
    GPIOPinConfigure(_uart[_ndx].rx_pin_mux);
}

void UART::disable()
{
    // Too much dereferencing. Let's minimize that a bit
    tPWM_Pin tx_pin = _uart[_ndx].tx_pin;
    tPWM_Pin rx_pin = _uart[_ndx].rx_pin;

    // Set GPIO Pin Mux
    GPIOPinTypeGPIOInput(tx_pin -> port.base, tx_pin -> offset);
    GPIOPinTypeGPIOInput(rx_pin -> port.base, rx_pin -> offset);
}

void UART::enableIRQ()
{

}

void UART::disableIRQ()
{

}

void UART::defaultIRQ()
{
    // Clear INT
    
}
