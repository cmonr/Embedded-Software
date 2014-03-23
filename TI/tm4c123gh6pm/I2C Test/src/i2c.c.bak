#include <stdbool.h>
#include <stdint.h>
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/i2c.h>
#include <driverlib/interrupt.h>


#include "i2c.h"


static tI2CRequestBuffer _i2cReqBuff;

void I2CInit(void)
{
    // Only init I2C0 for now
    
    // Power peripherals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);

    // Multiplex internal pins
    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);

    // Configure Pins
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);

    // Finish setup
    I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false);
    I2CMasterTimeoutSet(I2C0_BASE, 0xFF);

    // Initialize I2C Request Buffer
    _i2cReqBuff.head = 0;
    _i2cReqBuff.tail = 0;
    _i2cReqBuff.len = 0;

    // Enable I2C0 ISR
    I2CMasterIntEnableEx(I2C0_BASE, I2C_MASTER_INT_TIMEOUT |
                                    I2C_MASTER_INT_DATA);
    IntEnable(INT_I2C0);

    // Enable I2C0
    I2CMasterEnable(I2C0_BASE);
}


unsigned int I2CWrite_BusyWait(unsigned char addr, unsigned char* data, unsigned char len)
{
    tI2CRequest req;

    // Wait until buffer is empty
    while (_i2cReqBuff.len != 0);

    // Create Request
    I2C_buildRequest(addr, data, len, false);
    req = _i2cReqBuff.buff[_i2cReqBuff.head - 1];
    if (_i2cReqBuff.head == 0)
        req = _i2cReqBuff.buff[15];
    
    // Wait until buffer is empty again
    while (_i2cReqBuff.len != 0);

    return req.error;
}

void I2CWrite_Async(unsigned char addr, unsigned char* data, unsigned char len)
{
    I2C_buildRequest(addr, data, len, false);
}


unsigned int I2CRead_BusyWait(unsigned char addr, unsigned char* data, unsigned char len)
{
    tI2CRequest req;

    // Wait until buffer is empty
    while (_i2cReqBuff.len != 0);

    // Create Request
    I2C_buildRequest(addr, data, len, true);
    req = _i2cReqBuff.buff[_i2cReqBuff.head - 1];
    if (_i2cReqBuff.head == 0)
        req = _i2cReqBuff.buff[15];
    
    // Wait until buffer is empty again
    while (_i2cReqBuff.len != 0);

    return req.error;
}

void I2CRead_Async(unsigned char addr, unsigned char* data, unsigned char len)
{
    I2C_buildRequest(addr, data, len, true);
}



void I2C_buildRequest(unsigned char addr, unsigned char* data, unsigned char len, bool writing)
{
    // Check if Request Buffer is full
    if (_i2cReqBuff.len == 16)  
        return ;

    // Build Request
    (_i2cReqBuff.buff[_i2cReqBuff.head]).addr = addr;
    (_i2cReqBuff.buff[_i2cReqBuff.head]).data = data;
    (_i2cReqBuff.buff[_i2cReqBuff.head]).len = len;
    (_i2cReqBuff.buff[_i2cReqBuff.head]).writing = writing;
    (_i2cReqBuff.buff[_i2cReqBuff.head]).error = 0;
    
    // Start if request buffer is empty
    //  Otherwise, IRQ will auto increment requests
    if (_i2cReqBuff.len == 0)
        I2C_startRequest();

    // Increment Request Buffer
    _i2cReqBuff.head = (_i2cReqBuff.head + 1) & (16 - 1);
    _i2cReqBuff.len++;
}


void I2C_startRequest()
{
    tI2CRequest req = _i2cReqBuff.buff[_i2cReqBuff.tail];
  
    I2CMasterSlaveAddrSet(I2C0_BASE, req.addr, req.writing);
    I2CMasterDataPut(I2C0_BASE, *(req.data));

    // Send commands to I2C Peripheral to start
    if (req.writing)
        I2CMasterControl(I2C0_BASE, req.len == 1 ? I2C_MASTER_CMD_SINGLE_SEND : 
                                                      I2C_MASTER_CMD_BURST_SEND_START);
    else
        I2CMasterControl(I2C0_BASE, req.len == 1 ? I2C_MASTER_CMD_SINGLE_RECEIVE : 
                                                      I2C_MASTER_CMD_BURST_RECEIVE_START);

    // Sent a byte
    req.data++;
    req.len--;

    // ISR takes over from here
}


void I2C0IntHandler()
{
    unsigned long status = I2CMasterIntStatusEx(I2C0_BASE, false);
    tI2CRequest req = _i2cReqBuff.buff[_i2cReqBuff.tail];
    
    I2CMasterIntClearEx(I2C0_BASE, status);

    // Error occured for some reason
    //  Deal with it.
    if (status != I2C_MASTER_ERR_NONE)
    {
        // Request ended in error
        req.error = status;
        
         
        // if error is really bad
        //I2CReset();
       
        // Next item in request buffer
        _i2cReqBuff.tail = (_i2cReqBuff.tail + 1) & (16 - 1);
        _i2cReqBuff.len--;

        // Start next request if needed
        if (_i2cReqBuff.len != 0)
            I2C_startRequest();

        // If Request has Callback
        //req.callback();

        return;
    }
    
    // Request has been completed
    if (req.len == 0)
    {
        // Next item in request buffer
        _i2cReqBuff.len--;
        _i2cReqBuff.tail = (_i2cReqBuff.tail + 1) & (16 - 1);

        // Start next requests if needed
        if (_i2cReqBuff.len != 0)
            I2C_startRequest();

        // If Request has Callback
        //req.callback();

        return;
    }

    // Still more things to do with current request
    if (req.writing)
    {
        I2CMasterDataPut(I2C0_BASE, *(req.data));
        I2CMasterControl(I2C0_BASE, req.len == 1 ? I2C_MASTER_CMD_BURST_SEND_FINISH : 
                                                      I2C_MASTER_CMD_BURST_SEND_CONT);
        
    }else{
        *(req.data) = I2CMasterDataGet(I2C0_BASE);
        I2CMasterControl(I2C0_BASE, req.len == 1 ? I2C_MASTER_CMD_BURST_RECEIVE_FINISH : 
                                                      I2C_MASTER_CMD_BURST_RECEIVE_CONT);
    }

    req.data++;
    req.len--;
}
