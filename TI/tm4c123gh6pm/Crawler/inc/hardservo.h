#pragma once

#include "servo.h"

class HardServo : public Servo
{
    public:
        //unsigned int _pwm_period;
        unsigned int _pwm_width;
        unsigned int _pwm_offset;

        HardServo ( unsigned int, unsigned int );
        HardServo () {};

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

void initHardServos( void );
