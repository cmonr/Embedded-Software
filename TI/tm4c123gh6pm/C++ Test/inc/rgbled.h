class RGBLED
{
    public:
        float maxBrightness;

        RGBLED();

        void set( float, float, float );

    private:
        unsigned long _pwm_period;
};

