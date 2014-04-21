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

char _uarts_open = 0;

UART::UART(unsigned char ndx)
{
    // TODO: Add check for previous allocation
    //   For now, assume user is competent

    // Local variables
    _ndx = ndx;
    _format.baud = 115200;
    _format.length = EIGHT;
    _format.stop = ONE;
    _format.parity = NONE;

    if(!(_uarts_open & (1 << _ndx))) this->open();

}

UART::UART(unsigned char ndx, unsigned long baud)
{
    // TODO: Add check for previous allocation
    //   For now, assume user is competent

    // Local variables
    _ndx = ndx;
    _format.baud = baud;
    _format.length = EIGHT;
    _format.stop = ONE;
    _format.parity = NONE;

    if(!(_uarts_open & (1 << _ndx))) this->open();
}

UART::UART(unsigned char ndx, unsigned long baud, tDATA_LEN len, tSTOP_BITS stop, tPARITY_BIT parity)
{
    // TODO: Add check for previous allocation
    //   For now, assume user is competent

    // Local variables
    _ndx = _ndx;
    _format.baud = baud;
    _format.length = len;
    _format.stop = stop;
    _format.parity = parity;

    if(!(_uarts_open & (1 << _ndx))) this->open();

}

int UART::open()
{
  Pin tx_pin = Pin(_uart[_ndx].tx_pin);
  Pin rx_pin = Pin(_uart[_ndx].rx_pin);

    // Power UART Peripheral
    SysCtlPeripheralEnable(_uart[_ndx].periph);

    // Configure UART
    UARTConfigSetExpClk(_uart[_ndx].base, SysCtlClockGet(), _format.baud, _format.length | _format.stop | _format.parity);
    
    // Set GPIO Pin Mux
    GPIOPinTypeUART(tx_pin . port.base, tx_pin . offset);
    GPIOPinTypeUART(rx_pin . port.base, rx_pin . offset);
    GPIOPinConfigure(_uart[_ndx].tx_pin_mux);
    GPIOPinConfigure(_uart[_ndx].rx_pin_mux);

    // Enable UART
    UARTEnable(_uart[_ndx].base);
    
    // Disable internal FIFOs
    UARTFIFODisable(_uart[_ndx].base);
    return 0;
}

int UART::close()
{
  Pin tx = Pin(_uart[_ndx].tx_pin);
  Pin rx = Pin(_uart[_ndx].rx_pin);

    // Disable UART
    UARTDisable(_uart[_ndx].base);

    // Set GPIO Pin Mux
    tx.setDir(OUT);
    rx.setDir(OUT);
    return 0;
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


int UART::putc(int c)
{
    // Wait for space in FIFO
    //while(UARTSpaceAvail(_uart[_ndx].base) == false);

    // Put char into FIFO
  UARTCharPut(_uart[_ndx].base,(unsigned char) c);
    return 0;
}

int UART::getc()
{
  return (int) UARTCharGet(_uart[_ndx].base);
}

int UART::seek( long int offset, int origin) {
  return -1;
}

void UART0IntHandler(){ return; }
void UART1IntHandler(){ return; }
void UART2IntHandler(){ return; }
void UART3IntHandler(){ return; }
void UART4IntHandler(){ return; }
void UART5IntHandler(){ return; }
void UART6IntHandler(){ return; }
void UART7IntHandler(){ return; }
