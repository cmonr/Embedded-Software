class Motor
{
    public:
        unsigned int _pwm_period;

        Motor ( unsigned int, unsigned int );

        void set( float );
        void throttle( float );
        void invert( void );

    private:
        unsigned int _pwm_out;
        unsigned int _pwm_out_bit;
        bool _invert;
        float _throttle;
}

void initMotors( void );

void enableMotors( void );

void disableMotors( void );

