#include "pwm.h"


tPWM _pwm[2]=
{
    {SYSCTL_PERIPH_PWM0, PWM0_BASE,
        {
        {PB6, GPIO_PB6_M0PWM0, PWM_GEN_0, PWM_OUT_0, PWM_OUT_0_BIT},
        {PB7, GPIO_PB7_M0PWM1, PWM_GEN_0, PWM_OUT_1, PWM_OUT_1_BIT},
        {PB4, GPIO_PB4_M0PWM2, PWM_GEN_1, PWM_OUT_2, PWM_OUT_2_BIT},
        {PB5, GPIO_PB5_M0PWM3, PWM_GEN_1, PWM_OUT_3, PWM_OUT_3_BIT},
        {PE4, GPIO_PE4_M0PWM4, PWM_GEN_2, PWM_OUT_4, PWM_OUT_4_BIT},
        {PE5, GPIO_PE5_M0PWM5, PWM_GEN_2, PWM_OUT_5, PWM_OUT_5_BIT},
        {PC4, GPIO_PC4_M0PWM6, PWM_GEN_3, PWM_OUT_6, PWM_OUT_6_BIT},
        {PC5, GPIO_PC5_M0PWM7, PWM_GEN_3, PWM_OUT_7, PWM_OUT_7_BIT}
        }
    },
    {SYSCTL_PERIPH_PWM1, PWM1_BASE, 
        {
        {PD0, GPIO_PD0_M1PWM0, PWM_GEN_0, PWM_OUT_0, PWM_OUT_0_BIT},
        {PD1, GPIO_PD1_M1PWM1, PWM_GEN_0, PWM_OUT_1, PWM_OUT_1_BIT},
        {PA6, GPIO_PA6_M1PWM2, PWM_GEN_1, PWM_OUT_2, PWM_OUT_2_BIT},
        {PA7, GPIO_PA7_M1PWM3, PWM_GEN_1, PWM_OUT_3, PWM_OUT_3_BIT},
        {PF0, GPIO_PF0_M1PWM4, PWM_GEN_2, PWM_OUT_4, PWM_OUT_4_BIT},
        {PF1, GPIO_PF1_M1PWM5, PWM_GEN_2, PWM_OUT_5, PWM_OUT_5_BIT},
        {PF2, GPIO_PF2_M1PWM6, PWM_GEN_3, PWM_OUT_6, PWM_OUT_6_BIT},
        {PF3, GPIO_PF3_M1PWM7, PWM_GEN_3, PWM_OUT_7, PWM_OUT_7_BIT}
        }
    }
};

PWM* pwm[2];


PWM::PWM(unsigned char ndx, unsigned long freq)
{
    // TODO: Add check for previous allocation
    //   For now, assume user is competent
    pwm[ndx] = this;

    // Local Variables
    _ndx = ndx;
    _freq = freq;


    // Power PWM Peripheral
    SysCtlPeripheralEnable(_pwm[ndx].periph);

    // Power IO Peripheral for pin
    //SysCtlPeripheralEnable(_pins[_pwm[ndx].pin] -> port.periph);


    // Configure PWM
    //   Set clock divider
    SysCtlPWMClockSet(SYSCTL_PWMDIV_64);

    //   Disable All Outputs
    PWMOutputState(_pwm[ndx].base, 0xFF, false);

    //   Set all generator periods
    _pwm_period = SysCtlClockGet() / 64 / freq - 1;
    PWMGenPeriodSet(_pwm[ndx].base, PWM_GEN_0, _pwm_period);
    PWMGenPeriodSet(_pwm[ndx].base, PWM_GEN_1, _pwm_period);
    PWMGenPeriodSet(_pwm[ndx].base, PWM_GEN_2, _pwm_period);
    PWMGenPeriodSet(_pwm[ndx].base, PWM_GEN_3, _pwm_period);

    //   Set all generator modes
    PWMGenConfigure(_pwm[ndx].base, PWM_GEN_0, PWM_GEN_MODE_DOWN);
    PWMGenConfigure(_pwm[ndx].base, PWM_GEN_1, PWM_GEN_MODE_DOWN);
    PWMGenConfigure(_pwm[ndx].base, PWM_GEN_2, PWM_GEN_MODE_DOWN);
    PWMGenConfigure(_pwm[ndx].base, PWM_GEN_3, PWM_GEN_MODE_DOWN);
    
    //   Enable all generators
    PWMGenEnable(_pwm[ndx].base, PWM_GEN_0);
    PWMGenEnable(_pwm[ndx].base, PWM_GEN_1);
    PWMGenEnable(_pwm[ndx].base, PWM_GEN_2);
    PWMGenEnable(_pwm[ndx].base, PWM_GEN_3);
}


void PWM::set(unsigned char pin_ndx, float duty)
{
    if (duty > 1.0)
        duty = 1.0;
    else if (duty < 0.0)
        duty = 0.0;

    PWMPulseWidthSet(_pwm[_ndx].base, _pwm[_ndx].pins[pin_ndx].pwm_out, _pwm_period * duty);
}

void PWM::invert(unsigned char pin_ndx, bool inv)
{
   PWMOutputInvert(_pwm[_ndx].base, _pwm[_ndx].pins[pin_ndx].out_bit, inv); 
}


void PWM::enable(unsigned char pin_ndx)
{
    // Too much dereferencing. Let's minimize that a bit
    tPWM_Pin pwm_pin = _pwm[_ndx].pins[pin_ndx];

    // Enable Pin Port
    // TODO: Initialize this better?
    Pin(pwm_pin.pin);
    
    // Set GPIO Pin Mux
    GPIOPinTypePWM(pins[pwm_pin.pin] -> port.base, pins[pwm_pin.pin] -> offset);
    GPIOPinConfigure(pwm_pin.pin_mux);
    
    // Enable PWM Output
    PWMOutputState(_pwm[_ndx].base, pwm_pin.out_bit, true);
}

void PWM::disable(unsigned char pin_ndx)
{
    // Too much dereferencing. Let's minimize that a bit
    tPWM_Pin pwm_pin = _pwm[_ndx].pins[pin_ndx];

    // Disable PWM Output
    PWMOutputState(_pwm[_ndx].base, pwm_pin.out_bit, false);

    // Set GPIO Pin Mux
    GPIOPinTypeGPIOInput(_pins[pwm_pin.pin] -> port.base, _pins[pwm_pin.pin] -> offset);
}
