#include <stdbool.h>
#include <stdint.h>
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pwm.h>
#include <driverlib/pin_map.h>

#include "pin.h"

typedef struct{
        tPin pin;
        unsigned int pin_mux;

        unsigned int generator;
        unsigned int pwm_out;
        unsigned int out_bit;
    } tPWM_Pin;

typedef struct
{
    unsigned int periph;
    unsigned int base;


    tPWM_Pin pins[8];
} tPWM;


class PWM
{
    public:
        PWM( unsigned char, unsigned long );

        void set( unsigned char, float );
        void invert( unsigned char, bool );
        void enable( unsigned char );
        void disable( unsigned char );

    private:
        unsigned char _ndx;
        unsigned char _freq;
        unsigned char _pwm_period;
};

static PWM* pwm[2];
