extern "C"{
#include "pwm.h"
}

class Servo
{
    public:
        Servo ( tPWM*, unsigned char );

        void set( float );
        void setLimits( float, float );
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
