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
        void enable( void );
        void disable( void );

    private:
        bool _invert;
        float _offset;
        float _range;
};

void initSoftServos( void );

void attachSoftServo( SoftServo* );

extern "C" void WTimer5AIntHandler( void );

extern "C" void WTimer5BIntHandler( void );
