#include "ina220.h"

// Default Constructor. Do nothing.
INA220::INA220(){ }

INA220::INA220(tI2C* i2c, unsigned char offset)
{
    // Initialize local variables
    _i2c = i2c;
    _addr = (0x04 << 4) | offset;


    // Values on PowerOn Reset
    _reg[CONFIG] = 0x399F;
    _reg[SHUNT] = 0x0000;       // Read Only
    _reg[BUS] = 0x0000;         // Read Only
    _reg[POWER] = 0x0000;       // Read Only
    _reg[CURRENT] = 0x0000;     // Read Only
    _reg[CALIBRATION] = 0x0000;
}

unsigned short INA220::readVoltage()
{
    unsigned char tmp[2];
    // Unsure if this will work.
    //  Might need to tweak I2C functions to
    //  write without stop cond
    _buff[0] = BUS;
    I2C_Write(_i2c, _addr, _buff[0]);

    // Read Word
    I2C_ReadBuff(_i2c, _addr, tmp, 2); 
    _reg[BUS] = ((tmp[0] << 8) | tmp[1]) >> 3;

    return _reg[BUS];
}

