#define PART_TM4C1233H6PM 1

#include <stdbool.h>
#include <stdint.h>
#include <inc/hw_memmap.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/i2c.h>

#include "i2c.h"

static unsigned char _i2c_addr;

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

void I2CAddrSet(unsigned char addr)
{
    _i2c_addr = addr;
}

void I2CWrite(unsigned char *data, unsigned int len)
{
    I2CMasterSlaveAddrSet(I2C0_BASE, _i2c_addr, false);
    
    switch(len)
    {
        case 1:
          
          break;
        case 2:
          I2CMasterDataPut(I2C0_BASE, data[0]);
          I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
          while(I2CMasterBusy(I2C0_BASE));

          //if (I2CMasterErr(I2C0_BASE) != 0)
          //    return I2CMasterErr(I2C0_BASE); 
          
          I2CMasterDataPut(I2C0_BASE, data[1]);
          I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
          while(I2CMasterBusy(I2C0_BASE));

          break;

    }
}

void I2CRead(unsigned char *data, unsigned int len) {}
