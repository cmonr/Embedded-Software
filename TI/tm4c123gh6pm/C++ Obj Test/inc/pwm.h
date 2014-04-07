

typedef struct
{
    unsigned int periph;
    unsigned int base

    typedef struct{
        tPin pin;
        unsigned int pin_mux;

        unsigned int generator;
        unsigned int pwm_out;
        unsigned int out_bit;
    } tPWM_Pins pins[8];
} tPWM;


class PWM
{
    public:
        PWM( unsigned char, unsigned long );

        void set( unsigned char, float );
        void invert( unsigned char, bool );
        void enable( unsigned char );
        void disable( unsigned char );

    private:
        unsigned char _ndx;
        unsigned char _freq;
        unsigned char _pwm_period;
}

extern PWM* pwm[2];
