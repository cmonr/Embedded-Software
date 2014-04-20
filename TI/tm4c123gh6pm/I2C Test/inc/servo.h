class Servo
{
    public:
        unsigned int _pwm_period;

        Servo ( unsigned int, unsigned int );

        void set( float );
        void setLimits( float, float );
        void invert( void );
        void enable( void );
        void disable( void );

    private:
        unsigned int _pwm_out;
        unsigned int _pwm_out_bit;
        bool _invert;
        float _offset;
        float _range;
};

void initServos( void );
