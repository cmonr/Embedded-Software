#include "pwm.h"

#include "led.h"

#define MAX_LED_BRIGHTNESS 0.3

LED::LED(unsigned char pwm_ndx, unsigned char pwm_pin_ndx)
{
    
    _pwm_ndx = pwm_ndx;
    _pwm_pin_ndx = pwm_pin_ndx;
    
    set(0.0);
}

void LED::set(float duty)
{
    if (duty < 0.0)
        duty = 0.0;
    else if (duty > 1.0)
        duty = 1.0;
    
    duty = (duty * MAX_LED_BRIGHTNESS) * 0.99 + 0.005;

    pwm[_pwm_ndx].set(_pwm_pin_ndx, duty);
}

void LED::enable()
{   
    pwm[_pwm_ndx].enable();
}

void LED::disable()
{
    pwm[_pwm_ndx].disable();
}

