#include "servo.h"

// Default Constructor. Do nothing.
Servo::Servo(){ }

Servo::Servo(tPWM* pwm, unsigned char pwm_pin)
{
    // Initialize local variables
    _pwm = pwm;
    _pwm_pin = pwm_pin;

    _invert = false;

    limit(0.5/2.5, 2.5/2.5);    // Typical Servo range (in ms)

    set(0.5);
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
    PWM_Set(_pwm, _pwm_pin, duty / 8.0);

}

void Servo::limit(float min, float max)
{
    if (max < min)
         return;    // TODO: Fix this later

    // Calculate internal variables
    _offset = min;
    _range = max - min;
}

void Servo::invert()
{
    // Invert servo direction
    _invert = !_invert;
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

