#pragma once

#include "gservo.h"

class Servo : public GServo
{
    public:
        //unsigned int _pwm_period;
        unsigned int _pwm_width;
        unsigned int _pwm_offset;

        Servo ( unsigned int, unsigned int );
        Servo () {};

        void set( float );
        void setLimits( float, float );
        void invert( void );
        void enable( void );
        void disable( void );

    private:
        unsigned int _pwm_out;
        unsigned int _pwm_out_bit;
        bool _invert;
        float _offset;
        float _range;
};

void initServos( void );
