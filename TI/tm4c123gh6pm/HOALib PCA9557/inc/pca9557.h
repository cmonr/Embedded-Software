#include "i2c.h"

typedef struct
{
    P0, P1, P2, P3, P4, P5, P6, P7
} tPCA9557_Pin;


class PCA9557
{
    public:
        PCA9557( tI2C*, unsigned char = 0 );

        void set( tPCA9557_Pin, tPin_State );
        void setALL( tPCA9557_Pin, tPCA9557_Pin, tPCA9557_Pin, tPCA9557_Pin, tPCA9557_Pin, tPCA9557_Pin, tPCA9557_Pin, tPCA9557_Pin );
        void setALL(unsigned char, )
        void toggle( tPCA9557_Pin );
        unsigned char read( tPCA9557_Pin );
        unsigned char readALL( void );

    private:
        typedef enum
        {
            INPUT=0,
            OUTPUT,
            POLARIY,
            CONFIG
        } tPCA9557_Registers;

        tI2C0* _i2c;
        unsigned char _addr;
        unsigned char _buff[2];
        unsigned char _regs[4];
};
