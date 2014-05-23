#ifndef _INA220_
#define _INA220_

#include "i2c.h"

class INA220
{
    public:
        INA220( void ); // Default constructor. Does nothing.
        INA220( tI2C*, unsigned char = 0 );

        //void config( tINA220_Setting, );

        unsigned short readVoltage( void );

    private:
        typedef enum
        {
            CONFIG=0,
            SHUNT,
            BUS,
            POWER,
            CURRENT,
            CALIBRATION,
            INA220_NUM_REGS
        } tINA220_Registers;

        tI2C* _i2c;
        unsigned char _addr;
        unsigned short _reg[INA220_NUM_REGS];
        unsigned char _buff[INA220_NUM_REGS];
};

#endif
