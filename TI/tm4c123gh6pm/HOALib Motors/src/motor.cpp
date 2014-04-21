#include "drv8800.h"

DRV8800::DRV8800(tPWM *pwm, unsigned char pwm_pin, )
{
    
}

void DRV8800::set(float duty)
{
    // Bounds check
    if (duty < 0)
        duty = 0.0;
    else if (duty > 1.0)
        duty = 1.0;
    
    // Check for inversion
    if (_invert)
        duty = 1.0 - duty;

    // Calculate modified duty cycle
    duty = duty * _max;
    
    // Set PWM signal
    PWM_Set(_pwm, _pwm_pin, duty);

}

void DRV8800::throttle(float max)
{
    // Disable servo while configuring
    //disable();

    // Calculate internal variables
    _max = max;
    
    // Reset to neutral position 
    set(0.0);
}

void DRV8800::invert()
{
    //Disable servo while configuring
    //disable();

    // Invert servo direction
    _invert = !_invert;

    // Reset to neutral position 
    set(0.0);
}

void Servo::enable()
{
    // Enable Dir Pin

    // Enable PWM Output to Pin
    PWM_Enable(_pwm, _pwm_pin);

    // Enable signal
}

void Servo::disable()
{
    // Disable PWM Output to Pin
    PWM_Disable(_pwm, _pwm_pin);
}


