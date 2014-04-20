class LED
{
    public:
        unsigned int _pwm_period;

        LED ( unsigned int, unsigned int );

        void set( float );
        void enable( void );
        void disable( void );

    private:
        unsigned int _pwm_out;
        unsigned int _pwm_out_bit;
};

void initLEDs( void );
