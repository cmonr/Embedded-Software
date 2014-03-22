#include <stdint.h>
#include <inc/hw_memmap.h>
#include <inc/hw_gpio.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pwm.h>
#include <driverlib/pin_map.h>

#include "led.h"

#define MAX_LED_BRIGHTNESS 0.3

LED::LED(unsigned int pwm_out, unsigned int pwm_out_bit)
{
    _pwm_out = pwm_out;
    _pwm_out_bit = pwm_out_bit;
    _pwm_period = SysCtlClockGet() / 64 / 25000 - 1;
    
    set(1.0);
}

void LED::set(float duty)
{
    if (duty < 0)
        duty = 0.0;
    else if (duty > 1.0)
        duty = 1.0;
    
    duty = (duty * MAX_LED_BRIGHTNESS) * 0.99 + 0.005;

    PWMPulseWidthSet(PWM1_BASE, _pwm_out, _pwm_period * duty);
}

void LED::enable()
{
    PWMOutputState(PWM1_BASE, _pwm_out_bit, true);
}

void LED::disable()
{
    PWMOutputState(PWM1_BASE, _pwm_out_bit, false);
}


void initLEDs()
{
    unsigned int pwm_period;  

    // Power Peripherals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);

    // Initialize PWM IO
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIOPinConfigure(GPIO_PF1_M1PWM5);
    GPIOPinConfigure(GPIO_PF2_M1PWM6);
    GPIOPinConfigure(GPIO_PF3_M1PWM7);


    // Setup PWM Peripheral
    SysCtlPWMClockSet(SYSCTL_PWMDIV_64); // PWM Clock Divider
    pwm_period = SysCtlClockGet() / 64 / 25000 - 1; // 25KHz

    
    PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN);
    PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN);
    
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, pwm_period);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, pwm_period);

    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, 0);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 0);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, 0);

    // Invert PWM Outputs
    PWMOutputInvert(PWM1_BASE, PWM_OUT_5_BIT, false);
    PWMOutputInvert(PWM1_BASE, PWM_OUT_6_BIT, false);
    PWMOutputInvert(PWM1_BASE, PWM_OUT_7_BIT, false);

    // Disable PWM Outputs by default
    PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, false);
    PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, false);
    PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, false);

    // Enable PWM Generators
    PWMGenEnable(PWM1_BASE, PWM_GEN_2);
    PWMGenEnable(PWM1_BASE, PWM_GEN_3);
}

