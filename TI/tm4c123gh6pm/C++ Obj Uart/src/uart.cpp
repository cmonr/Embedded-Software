#include "uart.h"

// TODO: Local FIFOs

tUART _uart[8] = 
{
    {SYSCTL_PERIPH_UART0, UART0_BASE, PA1, PA0, GPIO_PA1_U0TX, GPIO_PA0_U0RX, INT_UART0},
    {SYSCTL_PERIPH_UART1, UART1_BASE, PB1, PB0, GPIO_PB1_U1TX, GPIO_PB0_U1RX, INT_UART1},
    {SYSCTL_PERIPH_UART2, UART2_BASE, PD7, PD6, GPIO_PD7_U2TX, GPIO_PD6_U2RX, INT_UART2},
    {SYSCTL_PERIPH_UART3, UART3_BASE, PC7, PC6, GPIO_PC7_U3TX, GPIO_PC6_U3RX, INT_UART3},
    {SYSCTL_PERIPH_UART4, UART4_BASE, PC5, PC4, GPIO_PC5_U4TX, GPIO_PC4_U4RX, INT_UART4},
    {SYSCTL_PERIPH_UART5, UART5_BASE, PE5, PE4, GPIO_PE5_U5TX, GPIO_PE4_U5RX, INT_UART5},
    {SYSCTL_PERIPH_UART6, UART6_BASE, PD5, PD4, GPIO_PD5_U6TX, GPIO_PD4_U6RX, INT_UART6},
    {SYSCTL_PERIPH_UART7, UART7_BASE, PE1, PE0, GPIO_PE1_U7TX, GPIO_PE0_U7RX, INT_UART7}
};

UART* uart[8];

UART::UART(unsigned char ndx)
{
    // TODO: Add check for previous allocation
    //   For now, assume user is competent
    uart[ndx] = this;

    // Local variables
    _ndx = ndx;
    _format.length = EIGHT;
    _format.stop = ONE;
    _format.parity = NONE;


    // Power UART Peripheral
    SysCtlPeripheralEnable(_uart[_ndx].periph);

    // Configure UART
    UARTConfigSetExpClk(_uart[_ndx].base, SysCtlClockGet(), 115200, _format.length | _format.stop | _format.parity);
}

UART::UART(unsigned char ndx, unsigned long baud)
{
    // TODO: Add check for previous allocation
    //   For now, assume user is competent
    uart[ndx] = this;

    // Local variables
    _ndx = ndx;
    _format.length = EIGHT;
    _format.stop = ONE;
    _format.parity = NONE;


    // Power UART Peripheral
    SysCtlPeripheralEnable(_uart[_ndx].periph);

    // Configure UART
    UARTConfigSetExpClk(_uart[_ndx].base, SysCtlClockGet(), baud, _format.length | _format.stop | _format.parity);
}

UART::UART(unsigned char ndx, unsigned long baud, tDATA_LEN len, tSTOP_BITS stop, tPARITY_BIT parity)
{
    // TODO: Add check for previous allocation
    //   For now, assume user is competent
    uart[ndx] = this;

    // Local variables
    _ndx = _ndx;
    _format.length = len;
    _format.stop = stop;
    _format.parity = parity;


    // Power UART Peripheral
    SysCtlPeripheralEnable(_uart[_ndx].periph);

    // Configure UART
    UARTConfigSetExpClk(_uart[_ndx].base, SysCtlClockGet(), baud, _format.length | _format.stop | _format.parity);
}

void UART::enable()
{
    tPin tx_pin = _uart[_ndx].tx_pin;
    tPin rx_pin = _uart[_ndx].rx_pin;

    // Enable Pin Port
    // TODO: Initialize this better?
    new Pin(tx_pin);
    new Pin(rx_pin);
    
    // Set GPIO Pin Mux
    GPIOPinTypeUART(pins[tx_pin] -> port.base, pins[tx_pin] -> offset);
    GPIOPinTypeUART(pins[rx_pin] -> port.base, pins[rx_pin] -> offset);
    GPIOPinConfigure(_uart[_ndx].tx_pin_mux);
    GPIOPinConfigure(_uart[_ndx].rx_pin_mux);

    // Enable UART
    UARTEnable(_uart[_ndx].base);
    
    // Disable internal FIFOs
    UARTFIFODisable(_uart[_ndx].base);
}

void UART::disable()
{
    // Too much dereferencing. Let's minimize that a bit
    tPin tx = _uart[_ndx].tx_pin;
    tPin rx = _uart[_ndx].rx_pin;

    // Disable UART
    UARTDisable(_uart[_ndx].base);

    // Set GPIO Pin Mux
    GPIOPinTypeGPIOInput(pins[tx] -> port.base, pins[tx] -> offset);
    GPIOPinTypeGPIOInput(pins[rx] -> port.base, pins[rx] -> offset);
}


void UART::setIRQHandler(tIRQ irq_type,  void (*irq)( void ))
{
    // Make sure interrupts are disabled
    disableIRQ();
    
    if (irq_type == TX)
    {
        _tx_irq = irq;
        _irq_mask |= UART_INT_TX;
    }
    else if (irq_type == RX)
    {
        _rx_irq = irq;
        _irq_mask |= UART_INT_RX;
    }
    
    // TODO: Consider adding ERROR IRQ Type
}

void UART::enableIRQ()
{
    // Enable Interrupts on UART Peripheral
    UARTIntEnable(_uart[_ndx].base, _irq_mask);

    // Enable UART Interrupt in NVIC
    IntEnable(_uart[_ndx].int_ndx);
}

void UART::disableIRQ()
{
    // Enable UART Interrupt in NVIC
    IntDisable(_uart[_ndx].int_ndx);

    // Enable Interrupts on UART Peripheral
    UARTIntDisable(_uart[_ndx].base, _irq_mask);
}

void UART::IRQ()
{
    // Get interrupt status
    volatile unsigned long status = UARTIntStatus(_uart[_ndx].base, true);

    // Clear status
    UARTIntClear(_uart[_ndx].base, status);

    // Do things according to interrupt
    switch(status)
    {
        case UART_INT_TX:
            _tx_irq();
        case UART_INT_RX:
            _rx_irq();
    }
}


void UART::write(unsigned char c)
{
    // Wait for space in FIFO
    //while(UARTSpaceAvail(_uart[_ndx].base) == false);

    // Put char into FIFO
    UARTCharPut(_uart[_ndx].base, c);
}

void UART::writeBuff(unsigned char* buff, unsigned long len)
{
    for(; len > 0; len--)
      this -> write(*buff++);
}

void UART::writeStr(unsigned char* str)
{
    // Write until End of String
    while(str != 0)
        this -> write(*str++);

    // Also send End of String char
    write(0);
}


unsigned char UART::read()
{
    return UARTCharGet(_uart[_ndx].base);
}

void UART::readBuff(unsigned char* buff, unsigned long len)
{
    for(; len > 0; len--)
       *buff++ = this -> read();
}

unsigned int UART::readStr(unsigned char* str, unsigned long len)
{
    // TODO: Implement?
    return 0;
}


void UART0IntHandler(){ uart[0] -> IRQ(); }
void UART1IntHandler(){ uart[1] -> IRQ(); }
void UART2IntHandler(){ uart[2] -> IRQ(); }
void UART3IntHandler(){ uart[3] -> IRQ(); }
void UART4IntHandler(){ uart[4] -> IRQ(); }
void UART5IntHandler(){ uart[5] -> IRQ(); }
void UART6IntHandler(){ uart[6] -> IRQ(); }
void UART7IntHandler(){ uart[7] -> IRQ(); }
