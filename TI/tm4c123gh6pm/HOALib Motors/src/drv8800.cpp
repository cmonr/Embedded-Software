#include <math.h>

#include "drv8800.h"

DRV8800::DRV8800(tPWM *pwm, unsigned char pwm_pin, tPinName dir)
{
    _pwm = pwm;
    _pwm_pin = pwm_pin;
    _nsleep = NONE;

    
    // Enable pins here to prevent an undefined output state 

    // Enable Dir Pin
    // BUG! Need bot LOW and HIGH to make motors work...
    Pin_Init(dir);
    Pin_Set(dir, LOW);
    Pin_Set(dir, HIGH);
    
    // Enable PWM Output to Pin
    PWM_Invert(_pwm, _pwm_pin, true);
    PWM_Enable(_pwm, _pwm_pin);
}

void DRV8800::set(float duty)
{
    // Bounds check (-1,1)
    if (duty < -1.0)
        duty = -1.0;
    else if (duty > 1.0)
        duty = 1.0;
    
    // Throttle duty cycle
    duty = fabs(duty / 2.0 + 0.5);
    
    // Set PWM signal
    PWM_Set(_pwm, _pwm_pin, duty);
}

void DRV8800::throttle(float max)
{
    // Disable servo while configuring
    //disable();

    // Check for valid range
    if (max < 0 || max > 1)
        return;

    // Set internal variables
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

void DRV8800::enable()
{
    Pin_Set(_nsleep, HIGH);
}

void DRV8800::disable()
{
    Pin_Set(_nsleep, LOW);
}


