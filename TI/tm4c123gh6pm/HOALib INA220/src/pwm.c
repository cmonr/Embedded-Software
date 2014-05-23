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
        },
        0, 0, 0, false
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
        },
        0, 0, 0, false
    }
};

unsigned long pwmClkDiv[7] =
{
    SYSCTL_PWMDIV_1,
    SYSCTL_PWMDIV_2,
    SYSCTL_PWMDIV_4,
    SYSCTL_PWMDIV_8,
    SYSCTL_PWMDIV_16,
    SYSCTL_PWMDIV_32,
    SYSCTL_PWMDIV_64
};


tPWM_ERR PWM_Init(tPWM* pwm, unsigned long freq)
{
    unsigned char div = 0;
    unsigned long tmp_reload;
    volatile unsigned char i;
    
    // Check if object already exists
    if (pwm -> isInit == true)
        return PWM_IS_INITIALIZED;

        
    // Both PWM Peripherals share a single clock which means
    //  that if two different frequencies are used, the smallest
    //  clock divisor needs to be used.

    // Reload value needed for desired freq
    tmp_reload = SysCtlClockGet() / freq;

    // Find the smallest clock divider needed for reload value
    //  Aka, what divisor is needed to make relaod value fit
    //  within 16 bit PWM Reload register?
    for(div=0; (tmp_reload >> div) > ((1 << 16)-1); div++)
        if (div == 6)
            // Reload value needed cannot be divided to fit within
            //  16 bits, and/or system clock is too fast
            return PWM_FREQ_TOO_LOW;


    // Get largest divisor needed for clocking both PWM peripherals
    for(i=0; i<2; i++)
    {
        // Only care about initialized modules
        if (_pwm[i].isInit == true)
        {
            // Current divisor is smaller than others
            if (div < _pwm[i].clkDiv)
                div = _pwm[i].clkDiv;
        }
    }

    // Update other PWM peripherals if able
    for(i=0; i<2; i++)
    {
        if (_pwm[i].isInit == true)
        {
            // Make sure other PWM peripherals can
            //  maintain a reasonable resolution
            if (((_pwm[i].reload) >> div) < PWM_MIN_RESOLUTION)
                // Well, shit.
                return PWM_INSUFFICIENT_RESOLUTION;

            // Update other PWM's divisor and reload values
            _pwm[i].reload = (((_pwm[i].reload + 1 /*+ 2*/) << _pwm[i].clkDiv) >> div) - 1/* - 2*/;
            _pwm[i].clkDiv = div;


            // Update PWM generators
            PWMGenPeriodSet(_pwm[i].base, PWM_GEN_0, _pwm[i].reload);
            PWMGenPeriodSet(_pwm[i].base, PWM_GEN_1, _pwm[i].reload);
            PWMGenPeriodSet(_pwm[i].base, PWM_GEN_2, _pwm[i].reload);
            PWMGenPeriodSet(_pwm[i].base, PWM_GEN_3, _pwm[i].reload);
        }
    }

    // Local Variables
    pwm -> clkDiv = div;
    pwm -> freq = freq;

    

    // Power PWM Peripheral
    SysCtlPeripheralEnable(pwm -> periph);

    // Configure PWM
    if (SysCtlPWMClockGet() != pwmClkDiv[0])
        SysCtlPWMClockSet(pwmClkDiv[0]);
    SysCtlPWMClockSet(pwmClkDiv[div]);
    
    //   Set all generator periods
    pwm -> reload = (tmp_reload >> div) - 1/* - 2*/;
    PWMGenPeriodSet(pwm -> base, PWM_GEN_0, pwm -> reload);
    PWMGenPeriodSet(pwm -> base, PWM_GEN_1, pwm -> reload);
    PWMGenPeriodSet(pwm -> base, PWM_GEN_2, pwm -> reload);
    PWMGenPeriodSet(pwm -> base, PWM_GEN_3, pwm -> reload);

    //   Set all generator modes
    PWMGenConfigure(pwm -> base, PWM_GEN_0, PWM_GEN_MODE_DOWN);
    PWMGenConfigure(pwm -> base, PWM_GEN_1, PWM_GEN_MODE_DOWN);
    PWMGenConfigure(pwm -> base, PWM_GEN_2, PWM_GEN_MODE_DOWN);
    PWMGenConfigure(pwm -> base, PWM_GEN_3, PWM_GEN_MODE_DOWN);
    
    //   Enable all generators
    PWMGenEnable(pwm -> base, PWM_GEN_0);
    PWMGenEnable(pwm -> base, PWM_GEN_1);
    PWMGenEnable(pwm -> base, PWM_GEN_2);
    PWMGenEnable(pwm -> base, PWM_GEN_3);

    //   Disable All Outputs
    PWMOutputState(pwm -> base, 0xFF, false);

    // Initialization completed
    pwm -> isInit = true;

	return PWM_NO_ERR;
}

void PWM_Set(tPWM* pwm, unsigned char pin_ndx, float duty)
{
    if (duty == 0.0)
        PWMOutputState(pwm -> base, pwm -> pins[pin_ndx].out_bit, false);
    else
    {
        if (duty == 1.0)
            PWMPulseWidthSet(pwm -> base, pwm -> pins[pin_ndx].pwm_out, pwm -> reload - 2);
        else
            PWMPulseWidthSet(pwm -> base, pwm -> pins[pin_ndx].pwm_out, (pwm -> reload) * duty);

        PWMOutputState(pwm -> base, pwm -> pins[pin_ndx].out_bit, true);
    }
}

void PWM_Invert(tPWM* pwm, unsigned char pin_ndx, bool inv)
{
   PWMOutputInvert(pwm -> base, pwm -> pins[pin_ndx].out_bit, inv); 
}


void PWM_Enable(tPWM* pwm, unsigned char pin_ndx)
{
    // Too much dereferencing. Let's minimize that a bit
    tPWM_Pin pwm_pin = pwm -> pins[pin_ndx];

    // Enable Pin Port
    // TODO: Initialize this better?
    Pin_Init(pwm_pin.pin);
    Pin_Set(pwm_pin.pin, LOW);
    
    // Set GPIO Pin Mux
    GPIOPinTypePWM(pins[pwm_pin.pin].port.base, pins[pwm_pin.pin].offset);
    GPIOPinConfigure(pwm_pin.pin_mux);
    
    // Enable PWM Output
    PWMOutputState(pwm -> base, pwm_pin.out_bit, true);
}

void PWM_Disable(tPWM* pwm, unsigned char pin_ndx)
{
    // Too much dereferencing. Let's minimize that a bit
    tPWM_Pin pwm_pin = pwm -> pins[pin_ndx];

    // Disable PWM Output
    PWMOutputState(pwm -> base, pwm_pin.out_bit, false);

    // Set GPIO Pin Mux
    GPIOPinTypeGPIOInput(pins[pwm_pin.pin].port.base, pins[pwm_pin.pin].offset);
}
