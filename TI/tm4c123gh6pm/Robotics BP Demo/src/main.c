#define PART_TM4C123GH6PM 1

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <driverlib/gpio.h>
#include <driverlib/uart.h>
#include <driverlib/i2c.h>
#include <driverlib/pin_map.h>
#include <driverlib/interrupt.h>
#include <driverlib/sysctl.h>


#include "motor.h"
#include "i2c.h"
#include "encoder.h"


#define toggleRed()    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, ~GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1))
#define toggleBlue()   GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, ~GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2))
#define toggleGreen()  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, ~GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_3))

#define delay(x)      SysCtlDelay(SysCtlClockGet() / 3 * x);


void BluetoothRXHandler(unsigned char);


unsigned char i2c_buff[2];


void init(void)
{
    // Clock (80MHz)
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    
    
    // GPIO
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);


    // UART (Computer Serial)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE | UART_CONFIG_WLEN_8));
    
    UARTEnable(UART0_BASE);
    UARTFIFODisable(UART0_BASE);
    setbuf(stdout, NULL);  // Disable printf buffer


    // UART (Android Bluetooth)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
    GPIOPinConfigure(GPIO_PB0_U1RX);
    GPIOPinConfigure(GPIO_PB1_U1TX);
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE | UART_CONFIG_WLEN_8));
    
    UARTEnable(UART1_BASE);
 

    
    // Motors
    I2CInit();

    // PCA9557
    i2c_buff[0] = 0x03;
    i2c_buff[1] = 0x00; // 0: Output   1: Input
    I2CWrite(0x18, i2c_buff, 2);  // IO Direction

    i2c_buff[0] = 0x02;
    i2c_buff[1] = 0x00;
    I2CWrite(0x18, i2c_buff, 2);  // IO Polarity

    i2c_buff[0] = 0x01;
    i2c_buff[1] = 0x0F | 0x00;    // Disable Motors
    I2CWrite(0x18, i2c_buff, 2);  // Output H/L

    initMotors();
    initEncoders();

    invertEncoder(0);


    // Enable Interrupts
    IntMasterEnable();
}


char btBuff[32];
unsigned char btBuffNdx = 0;
unsigned char ndx;
char sign;
int whole, dec;
float vel;
const char* vel_str;

int i;

int main(void)
{ 
    init();

    //i2c_buff[0] = 0x01;
    //i2c_buff[1] = 0x0F | 0x80;    // Enable Motors
    //I2CWrite(0x18, i2c_buff, 2);  // Output H/L
    
    // Start
    toggleRed();
    printf("\rScanning I2C Addresses\r\n");


    i2c_buff[0] = 0;

    for(i=0; i<0x7F; i++)
    {
        I2CWrite(i, i2c_buff, 1);

        SysCtlDelay(SysCtlClockGet()/10000);

        if (I2CMasterErr(I2C0_BASE) == I2C_MASTER_ERR_NONE)
        {
            printf("x%02X\r\n", i);
        }
    }

    // Finish
    printf("\r\nDone.\r\n");
    toggleRed();
    
    while(1);
}
