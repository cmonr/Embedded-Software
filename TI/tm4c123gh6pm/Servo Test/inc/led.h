class LED
{
    public:
        unsigned int _pwm_period;

        LED ( unsigned int, unsigned int );

        void set( float );
        void enable( bool );

    private:
        unsigned int _pwm_out;
        unsigned int _pwm_out_bit;
};

extern LED rLED, gLED, bLED;

void initLEDs( void );
