class Servo
{
    public:
        unsigned int _pwm_period;

        Servo ( unsigned int, unsigned int );

        void set( float );
        void setLimits( float, float );
        void invert( void );
        void enable( bool );

    private:
        unsigned int _pwm_out;
        unsigned int _pwm_out_bit;
        bool _invert;
        float _offset;
        float _range;
};

extern Servo servo0,
             servo1,
             servo2,
             servo3,
             servo4,
             servo5;

void initServos( void );
