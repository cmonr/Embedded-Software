#include "i2c.h"


tI2C _i2c[4] = 
{
    {SYSCTL_PERIPH_I2C0, I2C0_BASE, PB3, PB2, GPIO_PB3_I2C0SDA, GPIO_PB2_I2C0SCL, I2C_MODE_STD, false},
    {SYSCTL_PERIPH_I2C1, I2C1_BASE, PA7, PA6, GPIO_PA7_I2C1SDA, GPIO_PA6_I2C1SCL, I2C_MODE_STD, false},
    {SYSCTL_PERIPH_I2C2, I2C2_BASE, PE5, PE4, GPIO_PE5_I2C2SDA, GPIO_PE4_I2C2SCL, I2C_MODE_STD, false},
    {SYSCTL_PERIPH_I2C3, I2C3_BASE, PD1, PD0, GPIO_PD1_I2C3SDA, GPIO_PD0_I2C3SCL, I2C_MODE_STD, false}
};

tI2C_Err I2C_Init(tI2C* i2c)
{
     // Check if object already exists
    if (i2c -> isInit == true)
        return I2C_IS_INITIALIZED;

    // Power I2C Peripheral
    SysCtlPeripheralEnable(i2c -> periph);

    // Configure I2C
    I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false);

    //  Set timeout
    I2CMasterTimeoutSet(I2C0_BASE, 0xFF);

    // Init complete
    i2c -> isInit = true;

    return I2C_NO_ERR;
}

void I2C_SetMode(tI2C* i2c, tI2C_Mode mode)
{
    switch(mode)
    {
        case I2C_MODE_STD:
            I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false);
            break;
        case I2C_MODE_FAST:
            I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false);
            break;
        case I2C_MODE_FAST_PLUS:
        case I2C_MODE_HIGH_SPEED:
            // Ignore for now...
            return;
        default:
            return;
    }

    i2c -> mode = mode;
}

void I2C_Enable(tI2C* i2c)
{
    // Enable Pin Port
    Pin_Init(i2c -> sda_pin);
    Pin_Init(i2c -> scl_pin);
    
    // Set GPIO Pin Mux
    GPIOPinTypeI2C(pins[i2c -> sda_pin].port.base, pins[i2c -> sda_pin].offset);
    GPIOPinTypeI2CSCL(pins[i2c -> scl_pin].port.base, pins[i2c -> scl_pin].offset);
    GPIOPinConfigure(i2c -> sda_pin_mux);
    GPIOPinConfigure(i2c ->scl_pin_mux);

    // Enable I2C
    I2CMasterEnable(i2c -> base);
}

void I2C_Disable(tI2C* i2c)
{
    // Disable I2C
    I2CMasterDisable(i2c -> base);

    // Set GPIO Pin
    GPIOPinTypeGPIOInput(pins[i2c -> sda_pin].port.base, pins[i2c -> sda_pin].offset);
    GPIOPinTypeGPIOInput(pins[i2c -> scl_pin].port.base, pins[i2c -> scl_pin].offset);
}


bool I2C_Write(tI2C* i2c, unsigned char addr, unsigned char data)
{
    I2CMasterSlaveAddrSet(i2c -> base, addr, false);
    I2CMasterDataPut(i2c -> base, data);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
    while(I2CMasterBusy(I2C0_BASE));

    return I2CMasterErr(i2c -> base) == I2C_MASTER_ERR_NONE;
}

bool I2C_WriteBuff(tI2C* i2c, unsigned char addr, unsigned char* data, unsigned int len)
{
    I2CMasterSlaveAddrSet(i2c -> base, addr, false);
    I2CMasterDataPut(i2c -> base, *data);
    if (len == 1)
    {
        I2CMasterControl(i2c -> base, I2C_MASTER_CMD_SINGLE_SEND);
        while(I2CMasterBusy(i2c -> base));

        return I2CMasterErr(i2c -> base) == I2C_MASTER_ERR_NONE;
    }
    
    
    // Start sending consecutive data
    I2CMasterControl(i2c -> base, I2C_MASTER_CMD_BURST_SEND_START);
    while(I2CMasterBusy(i2c -> base));
    if (I2CMasterErr(i2c -> base) != I2C_MASTER_ERR_NONE)
        return false;
    len--;
    data++;

    
    // Continue sending consecutive data
    while(len > 1){
        I2CMasterDataPut(i2c -> base, *data);
        I2CMasterControl(i2c -> base, I2C_MASTER_CMD_BURST_SEND_CONT);
        while(I2CMasterBusy(i2c -> base));
        if (I2CMasterErr(i2c -> base) != I2C_MASTER_ERR_NONE)
            return false;
        len--;
        data++;
    }

    // Send last piece of data
    I2CMasterDataPut(i2c -> base, *data);
    I2CMasterControl(i2c -> base, I2C_MASTER_CMD_BURST_SEND_FINISH);
    while(I2CMasterBusy(i2c -> base));
   
    return I2CMasterErr(i2c -> base) == I2C_MASTER_ERR_NONE;
}


bool I2C_Read(tI2C* i2c, unsigned char addr, unsigned char* data)
{
    I2CMasterSlaveAddrSet(i2c -> base, addr, true);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
    while(I2CMasterBusy(I2C0_BASE));
    *data = I2CMasterDataGet(i2c -> base);

    return I2CMasterErr(i2c -> base) == I2C_MASTER_ERR_NONE;
}

bool I2C_ReadBuff(tI2C* i2c, unsigned char addr, unsigned char* data, unsigned int len)
{
    I2CMasterSlaveAddrSet(i2c -> base, addr, true);
    if (len == 1)
    {
        I2CMasterControl(i2c -> base, I2C_MASTER_CMD_SINGLE_RECEIVE);
        while(I2CMasterBusy(i2c -> base));
        *data = I2CMasterDataGet(i2c -> base);

        return I2CMasterErr(i2c -> base) == I2C_MASTER_ERR_NONE;
    }
    
    
    // Start receiving consecutive data
    I2CMasterControl(i2c -> base, I2C_MASTER_CMD_BURST_RECEIVE_START);
    while(I2CMasterBusy(i2c -> base));
    *data = I2CMasterDataGet(i2c -> base);
    if (I2CMasterErr(i2c -> base) != I2C_MASTER_ERR_NONE)
        return false;
    len--;
    data++;

    
    // Continue receiving consecutive data
    while(len > 1){
        I2CMasterControl(i2c -> base, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
        while(I2CMasterBusy(i2c -> base));
        *data = I2CMasterDataGet(i2c -> base);
        if (I2CMasterErr(i2c -> base) != I2C_MASTER_ERR_NONE)
            return false;
        len--;
        data++;
    }

    // Receive last piece of data
    I2CMasterControl(i2c -> base, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
    while(I2CMasterBusy(i2c -> base));
    *data = I2CMasterDataGet(i2c -> base);
   
    return I2CMasterErr(i2c -> base) == I2C_MASTER_ERR_NONE;
}

