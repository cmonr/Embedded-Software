extern "C" {
#include "i2c.h"
}

#include "pca9557.h"

PCA9557::PCA9557()
{
    PCA9557(0);
}

PCA9557::PCA9557(unsigned char offset)
{
    // Address
    addr = 0x18 | offset;
    
    // Disable all polarity inversion
    data[0] = 2;
    data[1] = 0x00;
    I2CWrite(addr, data, 2);

    // Init all IO to Inputs
    setDir(IN, IN, IN, IN, IN, IN, IN, IN);

    // All Outputs are initialized as Low
    _out = 0x00;
}

void PCA9557::setDir(IO_DIR p0, IO_DIR p1, IO_DIR p2, IO_DIR p3, IO_DIR p4, IO_DIR p5, IO_DIR p6, IO_DIR p7)
{
    _dir = (p7 << 7) |
           (p6 << 6) |
           (p5 << 5) |
           (p4 << 4) |
           (p3 << 3) |
           (p2 << 2) |
           (p1 << 1) |
           (p0 << 0);
    
    // Write changes to IC 
    data[0] = 3;
    data[1] = _dir;
    I2CWrite(addr, data, 2);
}

void PCA9557::setDir(unsigned char pin, IO_DIR dir)
{   
    // Clear and Set bit
    _dir &= ~(1 << pin);  
    _dir |= dir;
 
    // Write changes to IC 
    data[0] = 3;
    data[1] = _dir;
    I2CWrite(addr, data, 2);
}

void PCA9557::write(IO_LEVEL p0, IO_LEVEL p1, IO_LEVEL p2, IO_LEVEL p3, IO_LEVEL p4, IO_LEVEL p5, IO_LEVEL p6, IO_LEVEL p7)
{
    _out = (p7 << 7) |
           (p6 << 6) |
           (p5 << 5) |
           (p4 << 4) |
           (p3 << 3) |
           (p2 << 2) |
           (p1 << 1) |
           (p0 << 0);
    
    // Write changes to IC 
    data[0] = 1;
    data[1] = _out;
    I2CWrite(addr, data, 2);
}

void PCA9557::write(unsigned char pin, IO_LEVEL level)
{
    // Clear and Set bit
    _out &= ~(1 << pin);
    _out |= level;
    
    // Write changes to IC 
    data[0] = 1;
    data[1] = _dir;
    I2CWrite(addr, data, 2);
}

unsigned char PCA9557::read(unsigned char pin)
{
    unsigned char port;

    // Read Port
    port = read();
        
    // Isolate pin from port
    return (port >> pin) & 0x01;
}

unsigned char PCA9557::read()
{
    // Clear Buffer
    data[0] = 0;

    // Send command
    I2CRead(addr, data, 1);

    return data[0];
}
