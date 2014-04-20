#include "pca9557.h"

PCA9557::PCA9557(tI2C* i2c, unsigned char offset)
{
    // Initialize local variables
    _i2c = i2c;
    _addr = (0x18 << 3) | offset;
    

    // Set default states of registers
    //  TODO: Add check to make sure I2C is ready
    //  Make polarity uniform for all IO
    _buff[0] = POLARITY;
    _buff[1] = 0x00;
    I2C_WriteBuff(i2c, _addr, _buff, 2);
    //  TODO: Add check for success?

    //  Values on startup
    _reg[INPUT] = 0x00;
    _reg[OUTPUT] = 0x00;
    _reg[POLARITY] = 0xFF;
    _reg[CONFIG] = 0xFF;    // IO are inputs
}

void PCA9557::set(tPCA9557_Pin pin, tPin_State state)
{

}

void PCA9557::setALL(unsigned char dir, unsigned char state)
{
    // Set direction
    //  0: Output
    //  1: Input
    _buff[0] = CONFIG;
    _buff[1] = dir;
    I2C_WriteBuff(i2c, _addr, _buff, 2);
    
    // Set Output State
    //  0: LOW
    //  1: HIGH
    _buff[0] = OUTPUT;
    _buff[1] = state;
    I2C_WriteBuff(i2c, _addr, _buff, 2);
}

unsigned char PCA9557::readALL()
{
    // Unsure if this will work.
    //  Might need to tweak I2C functions to
    //  writing without stops
    _buff[0] = INPUT;
    I2C_WriteChar(i2c, _addr, _buff[0]);

    I2C_ReadChar(i2c, _addr, _regs[0]); 

    return _regs[0];
}
