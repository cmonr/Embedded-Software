/*



void (*UART1RXHandler)(unsigned char) = 0;

void UART1IntHandler(void){
  volatile unsigned long status = UARTIntStatus(UART1_BASE, true);

  while(UARTCharsAvail(UART1_BASE))
      UART1RXHandler(UARTCharGet(UART1_BASE));

  UARTIntClear(UART1_BASE, status);
}

void UART1WriteChar(unsigned char c){ UARTCharPut(UART1_BASE, c); }
void UART1Write(const unsigned char* buff, unsigned int len){
  while(len--)
      UARTCharPut(UART1_BASE, *buff++);
}


void BluetoothRXHandler(unsigned char c)
{
    UARTCharPut(UART0_BASE, c);
}



void initBluetooth()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);

    GPIOPinConfigure(GPIO_PB0_U1RX);
    GPIOPinConfigure(GPIO_PB1_U1TX);
 
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE | UART_CONFIG_WLEN_8));
    
    UARTEnable(UART1_BASE);
    UARTFIFODisable(UART1_BASE);

    UART1RXHandler = &BluetoothRXHandler;
    UARTIntEnable(UART1_BASE, UART_INT_RX);
    IntEnable(INT_UART1);


    // Test for correct Baud Rate
    // Try for Max (912600)

    // No Response. Try default (115200)

    // No Response. Try slowest (9600)
}

void bluetoothConnect()
{
    
}

void bluetoothMakeConnectable()
{
    
}

void bluetoothWrite(unsigned char c)
{

}

void bluetoothRead()
{

}*/
