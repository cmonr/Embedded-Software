#include "uart.h"

tUART _uart[8] = 
{
    {SYSCTL_PERIPH_UART0, UART0_BASE, PA1, PA0, GPIO_PA1_U0TX, GPIO_PA0_U0RX, INT_UART0, 0, {0,0}, false},
    {SYSCTL_PERIPH_UART1, UART1_BASE, PB1, PB0, GPIO_PB1_U1TX, GPIO_PB0_U1RX, INT_UART1, 0, {0,0}, false},
    {SYSCTL_PERIPH_UART2, UART2_BASE, PD7, PD6, GPIO_PD7_U2TX, GPIO_PD6_U2RX, INT_UART2, 0, {0,0}, false},
    {SYSCTL_PERIPH_UART3, UART3_BASE, PC7, PC6, GPIO_PC7_U3TX, GPIO_PC6_U3RX, INT_UART3, 0, {0,0}, false},
    {SYSCTL_PERIPH_UART4, UART4_BASE, PC5, PC4, GPIO_PC5_U4TX, GPIO_PC4_U4RX, INT_UART4, 0, {0,0}, false},
    {SYSCTL_PERIPH_UART5, UART5_BASE, PE5, PE4, GPIO_PE5_U5TX, GPIO_PE4_U5RX, INT_UART5, 0, {0,0}, false},
    {SYSCTL_PERIPH_UART6, UART6_BASE, PD5, PD4, GPIO_PD5_U6TX, GPIO_PD4_U6RX, INT_UART6, 0, {0,0}, false},
    {SYSCTL_PERIPH_UART7, UART7_BASE, PE1, PE0, GPIO_PE1_U7TX, GPIO_PE0_U7RX, INT_UART7, 0, {0,0}, false}
};

unsigned long uart_int_flags[UART_IRQ_FLAGS] =
{
    UART_INT_TX,
    UART_INT_RX
};

void UART_Init(tUART* uart)
{
    // Check if object already exists
    if (uart -> isInit == true)
        //return UART_IS_INITIALIZED;
        return;

    // Power UART Peripheral
    SysCtlPeripheralEnable(uart -> periph);

    // Configure UART
    UARTConfigSetExpClk(uart -> base, SysCtlClockGet(), 115200, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE );

    // Init complete
    uart -> isInit = true;
}

void UART_SetBaud(tUART* uart, unsigned int baud)
{
    UARTConfigSetExpClk(uart -> base, SysCtlClockGet(), baud, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE );
}

void UART_Enable(tUART* uart)
{
    // Enable Pin Port
    Pin_Init(uart -> tx_pin);
    Pin_Init(uart -> rx_pin);
    
    // Set GPIO Pin Mux
    GPIOPinTypeUART(pins[uart -> tx_pin].port.base, pins[uart -> tx_pin].offset);
    GPIOPinTypeUART(pins[uart -> rx_pin].port.base, pins[uart -> rx_pin].offset);
    GPIOPinConfigure(uart -> tx_pin_mux);
    GPIOPinConfigure(uart -> rx_pin_mux);

    // Enable UART
    UARTEnable(uart -> base);
    
    // Disable internal FIFOs
    UARTFIFODisable(uart -> base);
}

void UART_Disable(tUART* uart)
{
    // Disable UART
    UARTDisable(uart -> base);

    // Set GPIO Pin
    GPIOPinTypeGPIOInput(pins[uart -> tx_pin].port.base, pins[uart -> tx_pin].offset);
    GPIOPinTypeGPIOInput(pins[uart -> rx_pin].port.base, pins[uart -> rx_pin].offset);
}


void UART_WriteChar(tUART* uart, unsigned char c)
{
    // Wait for space in FIFO
    //while(UARTSpaceAvail(_uart[_ndx].base) == false);

    // Put char into FIFO
    UARTCharPut(uart -> base, c);
}

void UART_Write(tUART* uart, unsigned char* buff, unsigned long len)
{
    for(; len > 0; len--)
      UART_WriteChar(uart, *buff++);
}

void UART_WriteStr(tUART* uart, unsigned char* str)
{
    // Write until End of String
    while(str != 0)
        UART_WriteChar(uart, *str++);

    // Also send End of String char
    UART_WriteChar(uart, 0);
}


unsigned char UART_ReadChar(tUART* uart)
{
    return UARTCharGet(uart -> base);
}

void UART_Read(tUART* uart, unsigned char* buff, unsigned long len)
{
    for(; len > 0; len--)
       *buff++ = UART_ReadChar(uart);
}

unsigned int UART_ReadStr(tUART* uart, unsigned char* str, unsigned long len)
{
    // TODO: Implement?
    return 0;
}

void UART_SetIRQ(tUART* uart, tUART_IRQ_Flag flag, void (*func)(void))
{
    // Set handler
    uart -> irq[flag] = func;
}

void UART_IntEnable(tUART* uart, tUART_IRQ_Flag flag)
{
    // Make sure handler has been set up
    if (uart -> irq[flag] == 0)
        return;

    // Bookkeeping
    uart -> irq_flags |= uart_int_flags[flag];

    // Enable Interrupt
    UARTIntEnable(uart -> base, uart_int_flags[flag]);

    // Enable UART Int in NVIC
    IntEnable(uart -> nvic_ndx);
}

void UART_IntDisable(tUART* uart, tUART_IRQ_Flag flag)
{
    // Disable Interrupt
    UARTIntDisable(uart -> base, uart_int_flags[flag]);

    // Bookkeeping
    uart -> irq_flags &= ~uart_int_flags[flag];

    // No more UART Ints enabled
    if (uart -> irq_flags == 0)
        IntDisable(uart -> nvic_ndx);
}


void UART_IRQ(tUART* uart){
    // Get raw interrupt status
    volatile int flag = UARTIntStatus(uart -> base, true);

    // Ack Interrupt
    UARTIntClear(uart -> base, flag);

    // Handle the interrupt
    //   TODO: This is messy. Clean it up
    if (flag == uart_int_flags[0])    
        uart -> irq[0]();
    else if (flag == uart_int_flags[1])
        uart -> irq[1]();
}


void UART0IntHandler(){ UART_IRQ(&_uart[0]); }
void UART1IntHandler(){ UART_IRQ(&_uart[1]); }
void UART2IntHandler(){ UART_IRQ(&_uart[2]); }
void UART3IntHandler(){ UART_IRQ(&_uart[3]); }
void UART4IntHandler(){ UART_IRQ(&_uart[4]); }
void UART5IntHandler(){ UART_IRQ(&_uart[5]); }
void UART6IntHandler(){ UART_IRQ(&_uart[6]); }
void UART7IntHandler(){ UART_IRQ(&_uart[7]); }
