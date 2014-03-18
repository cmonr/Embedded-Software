#include <stdint.h>
#include <inc/hw_memmap.h>
#include <inc/hw_gpio.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pwm.h>
#include <driverlib/pin_map.h>

#include "led.h"

#define MAX_LED_BRIGHTNESS 0.3

LED rLED(PWM_OUT_5, PWM_OUT_5_BIT),
    bLED(PWM_OUT_7, PWM_OUT_7_BIT), 
    gLED(PWM_OUT_6, PWM_OUT_6_BIT);

LED::LED(unsigned int pwm_out, unsigned int pwm_out_bit)
{
    _pwm_out = pwm_out;
    _pwm_out_bit = pwm_out_bit;
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

void LED::enable(bool enable)
{
    PWMOutputState(PWM1_BASE,_pwm_out_bit, enable);
}


void initLEDs()
{
    unsigned int pwm_period;  

    // Turn on Peripherals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);


    // PWM
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIOPinConfigure(GPIO_PF1_M1PWM5);
    GPIOPinConfigure(GPIO_PF2_M1PWM6);
    GPIOPinConfigure(GPIO_PF3_M1PWM7);


    // Setup PWM Peripherals
    SysCtlPWMClockSet(SYSCTL_PWMDIV_64); // PWM Clock Divider
    pwm_period = SysCtlClockGet() / 64 / 25000 - 1; // 25KHz

    PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_UP_DOWN);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, pwm_period);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, pwm_period);
    PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, true);
    PWMGenEnable(PWM1_BASE, PWM_GEN_2);

    PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_UP_DOWN);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, pwm_period);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, pwm_period);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, pwm_period);
    PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, true);
    PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, true);
    PWMGenEnable(PWM1_BASE, PWM_GEN_3);

    // Invert PWM Outputs
    PWMOutputInvert(PWM1_BASE, PWM_OUT_5_BIT, false);
    PWMOutputInvert(PWM1_BASE, PWM_OUT_6_BIT, false);
    PWMOutputInvert(PWM1_BASE, PWM_OUT_7_BIT, false);

    // Finish init of objects
    rLED._pwm_period = pwm_period;
    gLED._pwm_period = pwm_period;
    bLED._pwm_period = pwm_period;
}

