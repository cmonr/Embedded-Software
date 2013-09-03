#define PART_TM4C1233H6PM 1

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <driverlib/gpio.h>
#include <driverlib/uart.h>
#include <driverlib/i2c.h>
#include <driverlib/pin_map.h>
#include <driverlib/interrupt.h>
#include <driverlib/sysctl.h>

//#include "adc.h"
#include "i2c.h"
//#include "servo.h"
#include "motor.h"
//#include "encoder.h"


#define toggleRed()   GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, ~GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1))
#define toggleBlue()  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, ~GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2))


unsigned char i2c_buff[2];

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

    //UARTEnable(UART0_BASE);
    //UARTFIFODisable(UART0_BASE);
   

   /* I2CInit();

    // PCA9557
    i2c_buff[0] = 0x03;
    i2c_buff[1] = 0x00; // 0: Output   1: Input
    I2CWrite(0x18, i2c_buff, 2);  // IO Direction

    i2c_buff[0] = 0x02;
    i2c_buff[1] = 0x00;
    I2CWrite(0x18, i2c_buff, 2);  // IO Polarity

    i2c_buff[0] = 0x01;
    i2c_buff[1] = 0x8F | 0x10;
    I2CWrite(0x18, i2c_buff, 2);  // Output H/L
*/
    I2CInit();


    //initMotors();
    //initServos();
    //initEncoders(false, false);
    //initLEDs();
    //initBluetooth();

    // Enable Interrupts
    IntMasterEnable();


    while(1)
    {
        unsigned int i;
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


        //UART1Write("A\r\n", 3);
        //UARTCharPut(UART1_BASE, 'B');
        
        //UART1WriteChar(UARTCharGet(UART0_BASE));
        
       /* 
        for (i=0; i<8; i++)
        {
            unsigned char tmp;  
          
            i2c_buff[0] = 0x84 | (i << 4);
            I2CWrite(0x48, i2c_buff, 1);
            if (I2CMasterErr(I2C0_BASE))
                printf("Err: %d\r\n", (unsigned int) I2CMasterErr(I2C0_BASE));

            I2CRead(0x48, &tmp, 1);
            if (I2CMasterErr(I2C0_BASE))
                printf("Err: %d\r\n", (unsigned int) I2CMasterErr(I2C0_BASE));


            printf("% 3d ", tmp);
        }*/

        toggleRed();

        SysCtlDelay(SysCtlClockGet() / 3 / 10);
      
       /* toggleRed();

        for(i=0; i<=100; i++)
        {
            setMotor(0, 0.01 * i);
            setMotor(1, 0.01 * i);
            setMotor(2, 0.01 * i);
            setMotor(3, 0.01 * i);
            printf("%d\r\n", i);
            SysCtlDelay(SysCtlClockGet() / 3 / 100);
        }

        toggleRed();

        for(; i>0; i--)
        {
            setMotor(0, 0.01 * i);
            setMotor(1, 0.01 * i);
            setMotor(2, 0.01 * i);
            setMotor(3, 0.01 * i);
            printf("%d\r\n", i);
            SysCtlDelay(SysCtlClockGet() / 3 / 100);
        }*/


        
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
