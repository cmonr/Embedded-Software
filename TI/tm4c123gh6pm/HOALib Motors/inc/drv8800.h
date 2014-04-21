#include "pwm.h"

class DRV8800
{
    public:
        DRB8800( tPWM*, unsigned char, tPin );

        void set( float );
        void throttle( float );
        void invert( void );

        void enable( void );
        void disable( void );

    private:
        tPWM* _pwm;
        unsigned char _pwm_pin;

        tPin _pin;
        bool _invert;
        float _max;
}
