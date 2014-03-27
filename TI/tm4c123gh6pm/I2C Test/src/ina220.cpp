extern "C" {
#include "i2c.h"
}

#include "ina220.h"

void INA220::INA220(float shuntResistance)
{
    _ohms = shuntResistance;
    _addr = 0x40;

    _init();
}

void INA220::INA220(unsigned char offset, float shuntResistance)
{
    _ohms = shuntResistance;
    _addr = 0x40 | offset;

    _init();
}

void INA220::_init(){
    regs.config = 0x399F;
    //regs.volts;
    //regs.amps;
    //regs.power;
    //regs.calibration;

    // Set PGA
    //  Default to 320mV
    regs.config &= ~(0x03 << 11);
    //regs.config &= ~(PGA_MASK);
    //regs.config |=  (PGA_40mV);
    //regs.config |=  (PGA_80mV);
    //regs.config |=  (PGA_160mV);
    regs.config |=  (PGA_320mV);

    // Set Bus Voltage Range
    //  Default to 32v
    regs.config &= ~(0x01 << 13);
    //regs.config |= (BUS_V_16v) << 13;
    regs.config |= (BUS_V_32v) << 13;


    // Set Bit Resolution (V)
    regs.config &= ~(0x0F << 7);
    regs.config &= ~(BIT_RES_V_MASK);
    //regs.config |= (BIT_RES_V_9);
    //regs.config |= (BIT_RES_V_10);
    //regs.config |= (BIT_RES_V_11);
    regs.config |= (BIT_RES_V_12);

    // Set Oversampling (V)
    //regs.config &= ~(0x0F << 7);
    //regs.config &= ~(OVER_V_MASK);
    //regs.config |= (OVER_V_1);
    //regs.config |= (OVER_V_2);
    //regs.config |= (OVER_V_4);
    //regs.config |= (OVER_V_8);
    //regs.config |= (OVER_V_16);
    //regs.config |= (OVER_V_32);
    //regs.config |= (OVER_V_64);
    //regs.config |= (OVER_V_128);

    
    // Set Bit Resolution (A)
    regs.config &= ~(0x0F << 3);
    regs.config &= ~(BIT_RES_V_MASK);
    //regs.config |= (BIT_RES_A_9);
    //regs.config |= (BIT_RES_A_10);
    //regs.config |= (BIT_RES_A_11);
    regs.config |= (BIT_RES_A_12);

    // Set Oversampling (A)
    //regs.config &= ~(0x0F << 3);
    //regs.config &= ~(OVER_A_MASK);
    //regs.config |= (OVER_A_1);
    //regs.config |= (OVER_A_2);
    //regs.config |= (OVER_A_4);
    //regs.config |= (OVER_A_8);
    //regs.config |= (OVER_A_16);
    //regs.config |= (OVER_A_32);
    //regs.config |= (OVER_A_64);
    //regs.config |= (OVER_A_128);

    
    // Set Mode
    regs.config &= ~(0x07 << 0);
    regs.config &= ~(MODE_MASK);
    regs.config |= MODE_OFF;        // 0
    regs.config |= MODE_V_TRIG;     // 1
    regs.config |= MODE_I_TRIG;     // 2
    regs.config |= MODE_VI_TRIG;    // 3
    regs.config |= MODE_IDLE;       // 4
    regs.config |= MODE_V_CONT;     // 5
    regs.config |= MODE_I_CONT;     // 6
    regs.config |= MODE_VI_CONT;    // 7

    
    // Reset INA220
    //regs.config |= (BUS_V_16v) << 13;
    regs.config |= RESET;
}

    // PGA =  40mV for Max I Resolution
    // PGA = 320mV for Max I Range

    // Calculate various params

    

    // Send them to I2C device


unsigned int INA220::setMaxVoltage(float maxV)
{

}
