class LED
{
    public:
        LED ( unsigned char, unsigned char );

        void set( float );
        void enable( void );
        void disable( void );

    private:
        unsigned int _pwm_ndx;
        unsigned int _pwm_pin_ndx;
};
