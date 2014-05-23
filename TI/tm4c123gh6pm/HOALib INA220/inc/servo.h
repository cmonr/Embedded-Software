#ifndef _SERVO_
#define _SERVO_

#include "pwm.h"

class Servo
{
    public:
        Servo ( void ); // Default constructor. Does nothing.
        Servo ( tPWM*, unsigned char );

        void set( float );
        void limit( float, float );
        void invert( void );
        void enable( void );
        void disable( void );

    private:
        tPWM* _pwm;
        unsigned char _pwm_pin;

        bool _invert;
        float _offset;
        float _range;
};

#endif
