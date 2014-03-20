class SoftServo
{
    public:
        unsigned int _pwm_period;
        unsigned int _port;
        unsigned int _pin;
        unsigned int _duration;

        SoftServo ( unsigned int, unsigned int );

        void set( float );
        void setLimits( float, float );
        void invert( void );
        void enable( bool );

    private:
        bool _invert;
        float _offset;
        float _range;
};

extern SoftServo servo6,
             servo7,
             servo8,
             servo9,
             servo10,
             servo11;

void initSoftServos( void );

extern "C" void WTimer5AIntHandler( void );

extern "C" void WTimer5BIntHandler( void );
