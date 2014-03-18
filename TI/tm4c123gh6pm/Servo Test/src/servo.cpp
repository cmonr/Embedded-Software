#include <stdint.h>
#include <inc/hw_memmap.h>
#include <inc/hw_gpio.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pwm.h>
#include <driverlib/pin_map.h>

#include "servo.h"

SERVO servo0(PWM_OUT_0, PWM_OUT_0_BIT),
      servo1(PWM_OUT_1, PWM_OUT_1_BIT),
      servo2(PWM_OUT_2, PWM_OUT_2_BIT),
      servo3(PWM_OUT_3, PWM_OUT_3_BIT),
      servo4(PWM_OUT_4, PWM_OUT_4_BIT),
      servo5(PWM_OUT_5, PWM_OUT_5_BIT);

SERVO::SERVO(unsigned int pwm_out, unsigned int pwm_out_bit)
{
    _pwm_out = pwm_out;
    _pwm_out_bit = pwm_out_bit;
}

void SERVO::set(float duty)
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
    PWMPulseWidthSet(PWM0_BASE, _pwm_out, _pwm_period * duty / 8.0);
}

void SERVO::setLimits(float min, float max)
{
    // Disable servo while configuring
    enable(false);

    // Calculate internal variables
    _offset = min;
    _range = max-min;
    
    // Reset to neutral position 
    set(0.5);
}

void SERVO::invert()
{
    // Invert servo direction
    _invert = !_invert;
}

void SERVO::enable(bool enable)
{
    // Enable PWM Output to Pin
    PWMOutputState(PWM0_BASE, _pwm_out_bit, enable);
}



void initServos()
{
    unsigned int pwm_period;  
    
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);


    // Setup IO
    GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    GPIOPinTypePWM(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    GPIOPinConfigure(GPIO_PB6_M0PWM0);
    GPIOPinConfigure(GPIO_PB7_M0PWM1);
    GPIOPinConfigure(GPIO_PB4_M0PWM2);
    GPIOPinConfigure(GPIO_PB5_M0PWM3);
    GPIOPinConfigure(GPIO_PE4_M0PWM4);
    GPIOPinConfigure(GPIO_PE5_M0PWM5);

    // Configure PWM
    SysCtlPWMClockSet(SYSCTL_PWMDIV_64);  // PWM Clock Divider
    pwm_period = SysCtlClockGet() / 64 / 50 - 1;  // 50Hz

    PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN);
    PWMGenConfigure(PWM0_BASE, PWM_GEN_1, PWM_GEN_MODE_DOWN);
    PWMGenConfigure(PWM0_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN);
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, pwm_period);
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, pwm_period);
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, pwm_period);
    
    
    // Enable PWM
    PWMGenEnable(PWM0_BASE, PWM_GEN_0);
    PWMGenEnable(PWM0_BASE, PWM_GEN_1);
    PWMGenEnable(PWM0_BASE, PWM_GEN_2);
   

    // Disable PWM Outputs
    PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT, false);
    PWMOutputState(PWM0_BASE, PWM_OUT_1_BIT, false);
    PWMOutputState(PWM0_BASE, PWM_OUT_2_BIT, false);
    PWMOutputState(PWM0_BASE, PWM_OUT_3_BIT, false);
    PWMOutputState(PWM0_BASE, PWM_OUT_4_BIT, false);
    PWMOutputState(PWM0_BASE, PWM_OUT_5_BIT, false);


    // Finish init of objects
    servo0._pwm_period = pwm_period;
    servo1._pwm_period = pwm_period;
    servo2._pwm_period = pwm_period;
    servo3._pwm_period = pwm_period;
    servo4._pwm_period = pwm_period;
    servo5._pwm_period = pwm_period;

    servo0.set(0.5);
    servo1.set(0.5);
    servo2.set(0.5);
    servo3.set(0.5);
    servo4.set(0.5);
    servo5.set(0.5);
}

