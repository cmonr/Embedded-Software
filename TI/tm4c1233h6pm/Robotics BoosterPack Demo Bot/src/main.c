#define PART_TM4C1233H6PM 1

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <driverlib/gpio.h>
#include <driverlib/uart.h>
#include <driverlib/pin_map.h>
#include <driverlib/interrupt.h>
#include <driverlib/sysctl.h>

//#include "adc.h"
//#include "i2c.h"
#include "servo.h"
//#include "motor.h"
//#include "encoder.h"


#define toggleRed()   GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, ~GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1))
#define toggleBlue()  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, ~GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2))

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



int main(void)
{ 
    // Clock (80MHz)
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    
    
    // GPIO
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);

    
    // UART (Serial)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE | UART_CONFIG_WLEN_8));
    
    UARTEnable(UART0_BASE);
   

    // UART (Bluetooth)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);

    GPIOPinConfigure(GPIO_PB0_U1RX);
    GPIOPinConfigure(GPIO_PB1_U1TX);
 
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE | UART_CONFIG_WLEN_8));
    
    UART1RXHandler = &BluetoothRXHandler;
    UARTIntEnable(UART1_BASE, UART_INT_RX);
    IntEnable(INT_UART1);
    UARTEnable(UART1_BASE);
    

    //I2CInit();

    /*I2CInit();
    I2CAddrSet(0x18);           // PCA9775
    I2CWrite([0x03, 0x0F], 2);  // IO Direction
    I2CWrite([0x02, 0x00], 2);  // IO Polarity
    I2CWrite([0x01, 0x80 | 0x10], 2);  // Output H/L
*/
    /*for(i=0; i<4; i++){
        I2CMasterSlaveAddrSet(I2C0_BASE, 0x18, false);
        I2CMasterDataPut(I2C0_BASE, 0x01);
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
        while(I2CMasterBusy(I2C0_BASE));

        I2CMasterSlaveAddrSet(I2C0_BASE, 0x18, true);
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
        while(I2CMasterBusy(I2C0_BASE));

        printf("x%0X\r\n", I2CMasterDataGet(I2C0_BASE));
    }*/


    //initMotors();
    initServos();
    //initEncoders();
    //initLEDs();

    // Enable Interrupts
    IntMasterEnable();


    while(1){
        // LED On
        //toggleRed();

        /*I2CMasterSlaveAddrSet(I2C0_BASE, 0x18, false);  // Set Outputs Directions
        I2CMasterDataPut(I2C0_BASE, 0x01);             
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
        while(I2CMasterBusy(I2C0_BASE));
        I2CMasterDataPut(I2C0_BASE, 0x00);            
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
        while(I2CMasterBusy(I2C0_BASE));


        I2CMasterSlaveAddrSet(I2C0_BASE, 0x18, false);  // Set Outputs Directions
        I2CMasterDataPut(I2C0_BASE, 0x01);             
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
        while(I2CMasterBusy(I2C0_BASE));
        I2CMasterDataPut(I2C0_BASE, 0xF0);            
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
        while(I2CMasterBusy(I2C0_BASE));
        */

        SysCtlDelay(SysCtlClockGet() / 3 / 2);
        toggleRed();
        //UART1Write("A\r\n", 3);
        UARTCharPut(UART1_BASE, 'B');

        //printf("%d\r\n", (unsigned int) (((ADCRead(3) >> 4) - 45) * 0.45));
        //setServo(0, (((ADCRead(3) >> 4) - 45) * 0.45) / 100.0);

/*
        I2CMasterSlaveAddrSet(I2C0_BASE, 0x18, false);  // Set Outputs Directions
        I2CMasterDataPut(I2C0_BASE, 0x01);             
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
        while(I2CMasterBusy(I2C0_BASE));
        if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) == 0)
            I2CMasterDataPut(I2C0_BASE, 0x80);            
        else
            I2CMasterDataPut(I2C0_BASE, 0x70);            
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
        while(I2CMasterBusy(I2C0_BASE));*/


        /*I2CMasterSlaveAddrSet(I2C0_BASE, 0x18, false);
        I2CMasterDataPut(I2C0_BASE, 0x00);
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
        while(I2CMasterBusy(I2C0_BASE));

        I2CMasterSlaveAddrSet(I2C0_BASE, 0x18, true);
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
        while(I2CMasterBusy(I2C0_BASE));

        if( (I2CMasterDataGet(I2C0_BASE) & 0x01) != 0)
            toggleBlue();*/
    }
}
