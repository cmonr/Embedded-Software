#include <stdint.h>
#include <inc/hw_memmap.h>
#include <inc/hw_gpio.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pwm.h>
#include <driverlib/pin_map.h>

#include "rgbled.h"

RGBLED::RGBLED()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);


    // PWM
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIOPinConfigure(GPIO_PF1_M1PWM5);
    GPIOPinConfigure(GPIO_PF2_M1PWM6);
    GPIOPinConfigure(GPIO_PF3_M1PWM7);


    // Setup PWM Peripherals
    SysCtlPWMClockSet(SYSCTL_PWMDIV_64); // PWM Clock Divider
    _pwm_period = SysCtlClockGet() / 64 / 25000 - 1; // 25KHz

    PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_UP_DOWN);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, _pwm_period);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, 0);
    PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, true);
    PWMGenEnable(PWM1_BASE, PWM_GEN_2);

    PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_UP_DOWN);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, _pwm_period);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 0);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, 0);
    PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, true);
    PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, true);
    PWMGenEnable(PWM1_BASE, PWM_GEN_3);

    // Invert PWM Outputs
    PWMOutputInvert(PWM1_BASE, PWM_OUT_5_BIT, false);
    PWMOutputInvert(PWM1_BASE, PWM_OUT_6_BIT, false);
    PWMOutputInvert(PWM1_BASE, PWM_OUT_7_BIT, false);

    // Setup local variables
    maxBrightness = 0.3;
    set(1,1,1);
}

void RGBLED::set(float r, float g, float b)
{
    if (r < 0)
        r = 0.0;
    else if (r > 1.0)
        r = 1.0;

    if (g < 0)
        g = 0.0;
    else if (g > 1.0)
        g = 1.0;

    if (b < 0)
        b = 0.0;
    else if (b > 1.0)
        b = 1.0;
    
    r = (r*maxBrightness)*0.99 + 0.005;
    b = (b*maxBrightness)*0.99 + 0.005;
    g = (g*maxBrightness)*0.99 + 0.005;


    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, _pwm_period * r);    // Red LED
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, _pwm_period * g);    // Green LED
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, _pwm_period * b);    // Blue LED
}

