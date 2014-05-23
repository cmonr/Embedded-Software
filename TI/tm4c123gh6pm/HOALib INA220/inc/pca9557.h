#ifndef _PCA9557_
#define _PCA9557_

#include "i2c.h"

typedef enum
{
    P0, P1, P2, P3, P4, P5, P6, P7
} tPCA9557_Pin;

class PCA9557
{
    public:
        PCA9557( void ); // Default constructor. Does nothing.
        PCA9557( tI2C*, unsigned char = 0 );

        void set( tPCA9557_Pin, tPin_State );
        void set( unsigned char, unsigned char );

        tPin_State read( tPCA9557_Pin );
        unsigned char read( void );

        void toggle( tPCA9557_Pin );

    private:
        typedef enum
        {
            INPUT=0,
            OUTPUT,
            POLARITY,
            CONFIG
        } tPCA9557_Registers;

        tI2C* _i2c;
        unsigned char _addr;
        unsigned char _buff[2];
        unsigned char _reg[4];
};

#endif
