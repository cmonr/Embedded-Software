#include "servo.h"

Servo::Servo(tPWM* pwm, unsigned char pwm_pin)
{
    // Initialize local variables
    _pwm = pwm;
    _pwm_pin = pwm_pin;

    setLimits(0, 1);
}

void Servo::set(float duty)
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
    duty = _offset + duty * _range;
    
    // Set PWM signal
    PWM_Set(_pwm, _pwm_pin, duty / 8);

}

void Servo::setLimits(float min, float max)
{
    // Disable servo while configuring
    //disable();

    // Calculate internal variables
    _offset = min;
    _range = max-min;
    
    // Reset to neutral position 
    set(0.5);
}

void Servo::invert()
{
    //Disable servo while configuring
    //disable();

    // Invert servo direction
    _invert = !_invert;

    // Reset to neutral position 
    set(0.5);
}

void Servo::enable()
{
    // Enable PWM Output to Pin
    PWM_Enable(_pwm, _pwm_pin);
}

void Servo::disable()
{
    // Disable PWM Output to Pin
    PWM_Disable(_pwm, _pwm_pin);
}

