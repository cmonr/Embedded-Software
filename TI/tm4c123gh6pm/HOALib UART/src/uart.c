#include "uart.h"

// TODO: Local FIFOs

tUART uart[8] = 
{
    {SYSCTL_PERIPH_UART0, UART0_BASE, PA1, PA0, GPIO_PA1_U0TX, GPIO_PA0_U0RX, INT_UART0, 115200, false},
    {SYSCTL_PERIPH_UART1, UART1_BASE, PB1, PB0, GPIO_PB1_U1TX, GPIO_PB0_U1RX, INT_UART1, 115200, false},
    {SYSCTL_PERIPH_UART2, UART2_BASE, PD7, PD6, GPIO_PD7_U2TX, GPIO_PD6_U2RX, INT_UART2, 115200, false},
    {SYSCTL_PERIPH_UART3, UART3_BASE, PC7, PC6, GPIO_PC7_U3TX, GPIO_PC6_U3RX, INT_UART3, 115200, false},
    {SYSCTL_PERIPH_UART4, UART4_BASE, PC5, PC4, GPIO_PC5_U4TX, GPIO_PC4_U4RX, INT_UART4, 115200, false},
    {SYSCTL_PERIPH_UART5, UART5_BASE, PE5, PE4, GPIO_PE5_U5TX, GPIO_PE4_U5RX, INT_UART5, 115200, false},
    {SYSCTL_PERIPH_UART6, UART6_BASE, PD5, PD4, GPIO_PD5_U6TX, GPIO_PD4_U6RX, INT_UART6, 115200, false},
    {SYSCTL_PERIPH_UART7, UART7_BASE, PE1, PE0, GPIO_PE1_U7TX, GPIO_PE0_U7RX, INT_UART7, 115200, false}
};

void UART_Init(tUART uart)
{
    // Check if object already exists
    if (uart.isInit == true)
        //return UART_IS_INITIALIZED;
        return;

    // Power UART Peripheral
    SysCtlPeripheralEnable(uart.periph);

    // Configure UART
    UARTConfigSetExpClk(uart.base, SysCtlClockGet(), 115200, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE );

    // Init complete
    uart.isInit = true;
}

void UART_Enable(tUART uart)
{
    // Enable Pin Port
    IO_Init(uart.tx_pin);
    IO_Init(uart.rx_pin);
    
    // Set GPIO Pin Mux
    GPIOPinTypeUART(pins[uart.tx_pin].port.base, pins[uart.tx_pin].offset);
    GPIOPinTypeUART(pins[uart.rx_pin].port.base, pins[uart.rx_pin].offset);
    GPIOPinConfigure(uart.tx_pin_mux);
    GPIOPinConfigure(uart.rx_pin_mux);

    // Enable UART
    UARTEnable(uart.base);
    
    // Disable internal FIFOs
    UARTFIFODisable(uart.base);
}

void UART_Disable(tUART uart)
{
    // Disable UART
    UARTDisable(uart.base);

    // Set GPIO Pin Mux
    GPIOPinTypeGPIOInput(pins[uart.tx_pin].port.base, pins[uart.tx_pin].offset);
    GPIOPinTypeGPIOInput(pins[uart.rx_pin].port.base, pins[uart.rx_pin].offset);
}
/*

void UART_IntConfig(tIRQ irq_type,  void (*irq)( void ))
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
}*/


void UART_WriteChar(tUART uart, unsigned char c)
{
    // Wait for space in FIFO
    //while(UARTSpaceAvail(_uart[_ndx].base) == false);

    // Put char into FIFO
    UARTCharPut(uart.base, c);
}

void UART_Write(tUART uart, unsigned char* buff, unsigned long len)
{
    for(; len > 0; len--)
      UART_WriteChar(uart, *buff++);
}

void UART_WriteStr(tUART uart, unsigned char* str)
{
    // Write until End of String
    while(str != 0)
        UART_WriteChar(uart, *str++);

    // Also send End of String char
    UART_WriteChar(uart, 0);
}


unsigned char UART_ReadChar(tUART uart)
{
    return UARTCharGet(uart.base);
}

void UART_Read(tUART uart, unsigned char* buff, unsigned long len)
{
    for(; len > 0; len--)
       *buff++ = UART_ReadChar(uart);
}

unsigned int UART_ReadStr(tUART uart, unsigned char* str, unsigned long len)
{
    // TODO: Implement?
    return 0;
}


void UART0IntHandler(){ }
void UART1IntHandler(){ }
void UART2IntHandler(){ }
void UART3IntHandler(){ }
void UART4IntHandler(){ }
void UART5IntHandler(){ }
void UART6IntHandler(){ }
void UART7IntHandler(){ }
