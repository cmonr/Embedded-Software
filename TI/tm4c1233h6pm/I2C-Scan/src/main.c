#define PART_TM4C123GH6PM 1

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <inc/hw_memmap.h>
#include <driverlib/debug.h>
#include <driverlib/gpio.h>
#include <driverlib/uart.h>
#include <driverlib/i2c.h>
#include <driverlib/rom.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>


int i;
int timeout;

int main(void)
{
    // Clock (80MHz)
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    
    
    // GPIO
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);

    
    // UART
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE | UART_CONFIG_WLEN_8));
    
    UARTEnable(UART0_BASE);
    

    // I2C
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
    I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false);
    
    I2CMasterEnable(I2C0_BASE);
    

    
    // Scan for addresses
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
    
    printf("Scan started\r\n");

    for(i=0; i<255; i++)
    {
        I2CMasterSlaveAddrSet(I2C0_BASE, i>>1, false);
        I2CMasterDataPut(I2C0_BASE, 0);
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);

        SysCtlDelay(SysCtlClockGet()/10000);

        if (I2CMasterErr(I2C0_BASE) ==  I2C_MASTER_ERR_NONE)
        {
            printf("x%02X\r\n", i);
        }
        else
        {
            //printf(" N\r\n");
        }
    }

    printf("\r\nScan complete\r\n\r\n");

    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);



    while(1);
}
