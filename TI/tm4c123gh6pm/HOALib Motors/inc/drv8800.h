#include "pwm.h"

class DRV8800
{
    public:
        DRV8800( tPWM*, unsigned char, tPinName );

        void set( float );
        void throttle( float );
        void invert( void );

        void enable( void );
        void disable( void );

    private:
        tPWM* _pwm;
        unsigned char _pwm_pin;

        tPinName _dir;
        tPinName _nsleep;
        bool _invert;
        float _max;

        long debug;
};
