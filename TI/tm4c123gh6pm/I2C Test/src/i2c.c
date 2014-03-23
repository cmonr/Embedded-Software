#include <stdbool.h>
#include <stdint.h>
#include <inc/hw_memmap.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/i2c.h>

#include "i2c.h"

void I2CInit(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);

    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);

    I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), true);
    I2CMasterTimeoutSet(I2C0_BASE, 0xFF);
    
    //I2CMmasterIntEnable(I2C0_BASE);
    I2CMasterEnable(I2C0_BASE);
    //IntEnable(INT_I2C0);
}


void I2CWrite(unsigned char addr, unsigned char *data, unsigned int len)
{
    I2CMasterSlaveAddrSet(I2C0_BASE, addr, false);
    I2CMasterDataPut(I2C0_BASE, *data);
    if (len == 1){
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
        while(I2CMasterBusy(I2C0_BASE));
        return;
    }

    // Start sending consecutive data
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    while(I2CMasterBusy(I2C0_BASE));
    len--;
    data++;

    // Continue sending consecutive data
    while(len > 1){
        I2CMasterDataPut(I2C0_BASE, *data);
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
        while(I2CMasterBusy(I2C0_BASE));
        len--;
        data++;
    }

    // Send last piece of data
    I2CMasterDataPut(I2C0_BASE, *data);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    while(I2CMasterBusy(I2C0_BASE));
}


void I2CRead(unsigned char addr, unsigned char* data, unsigned int len)
{
    if (len < 1)  // Assume I2C Recieving will always return data
        return;

    // Set address to read from
    I2CMasterSlaveAddrSet(I2C0_BASE, addr, true);

    // Check to see if pointer is to an array
    if (len == 1){
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
        while(I2CMasterBusy(I2C0_BASE));
        *data = I2CMasterDataGet(I2C0_BASE);
        return;
    }

    // Begin reading consecutive data
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
    while(I2CMasterBusy(I2C0_BASE));
    *data = I2CMasterDataGet(I2C0_BASE);
    len--;
    data++;

    // Continue reading consecutive data
    while(len > 1){
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
        while(I2CMasterBusy(I2C0_BASE));
        *data = I2CMasterDataGet(I2C0_BASE);
        len--;
        data++;
    }

    // Read last character of data  
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
    while(I2CMasterBusy(I2C0_BASE));
    *data = I2CMasterDataGet(I2C0_BASE);
}
