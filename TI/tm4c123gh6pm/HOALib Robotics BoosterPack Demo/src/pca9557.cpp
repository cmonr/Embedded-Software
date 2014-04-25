#include "pca9557.h"

// Default constructor. Do nothing.
PCA9557::PCA9557(){ }

PCA9557::PCA9557(tI2C* i2c, unsigned char offset)
{
    // Initialize local variables
    _i2c = i2c;
    _addr = (0x03 << 3) | offset;
    

    // Set default states of registers
    //  TODO: Add check to make sure I2C is ready
    //  Make polarity uniform for all IO
    _buff[0] = POLARITY;
    _buff[1] = 0x00;
    I2C_WriteBuff(_i2c, _addr, _buff, 2);
    //  TODO: Add check for success?

    //  Values on startup
    _reg[INPUT] = 0x00;
    _reg[OUTPUT] = 0x00;
    _reg[POLARITY] = 0x00;
    _reg[CONFIG] = 0xFF;
}

void PCA9557::set(tPCA9557_Pin pin, tPin_State state)
{
    unsigned char dir, out;

    // Check for input/output modes
    if (state == HiZ)
    {
        // Change pin to output
        dir = _reg[CONFIG] | (1 << pin);

        // Output state remains unchanged
        out = _reg[OUTPUT];
    }
    else
    {
        // Set pin config
        dir = _reg[CONFIG] & ~(1 << pin);

        // Clear and set output state
        out = _reg[OUTPUT] & ~(1 << pin);
        out |= (state << pin);
    }

    // Update registers
    set(dir, out);
}

void PCA9557::set(unsigned char dir, unsigned char out)
{
    // Only update direction if different
    if (dir != _reg[CONFIG])
    {
        // Set direction
        //  0: Output
        //  1: Input
        _buff[0] = CONFIG;
        _buff[1] = dir;
        I2C_WriteBuff(_i2c, _addr, _buff, 2);

        // Save state
        _reg[CONFIG] = dir;
    }
    
    // Only update output states if different
    if (out != _reg[OUTPUT])
    {
        // Set Output State
        //  0: LOW
        //  1: HIGH
        _buff[0] = OUTPUT;
        _buff[1] = out;
        I2C_WriteBuff(_i2c, _addr, _buff, 2);

        // Save state
        _reg[OUTPUT] = out;
    }
}

unsigned char PCA9557::read()
{
    // Unsure if this will work.
    //  Might need to tweak I2C functions to
    //  write without stop cond
    _buff[0] = INPUT;
    I2C_Write(_i2c, _addr, _buff[0]);

    I2C_Read(_i2c, _addr, &_reg[INPUT]); 

    return _reg[INPUT];
}

tPin_State PCA9557::read(tPCA9557_Pin pin)
{
    // Return single bit
    return (read() & (1 << pin)) == 0 ? LOW : HIGH;
}

void PCA9557::toggle(tPCA9557_Pin pin)
{
    // Set output
    set(_reg[CONFIG], _reg[OUTPUT] ^ (1 << pin));
}
