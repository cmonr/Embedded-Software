#include "pwm.h"

tPWM pwm[2]=
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


tPWM_ERR PWM_Init(unsigned char ndx, unsigned long freq)
{
    unsigned char div;
    unsigned long reload;

    
    // Check if object already exists
    if (pwm[ndx].isInit == true)
        return PWM_IS_INITIALIZED;
	
    // Find best clk div for PWM periphs
    reload = SysCtlClockGet() / freq;
    //   Find smallest clk div
    for(div=0; (reload >> div) > ((1 <<16)-1); div++)
        if (div == 6)
            return PWM_FREQ_TOO_LOW;

    pwm[ndx].clkDiv = div;

    //   Check other PWM freq
    //     Only worry about this if the other PWM periph is initialized
    if (pwm[1-ndx].isInit == true)
    {
        // Check for larger divider
        if (pwm[1-ndx].clkDiv > pwm[ndx].clkDiv)
            // Make sure larger divider would result in sufficient PWM resolution
            if ((reload >> (pwm[1-ndx].clkDiv)) < PWM_MIN_RESOLUTION)
                return PWM_INSUFFICIENT_RESOLUTION; 
    }

    // Local Variables
    pwm[ndx].freq = freq;

    // Power PWM Peripheral
    SysCtlPeripheralEnable(pwm[ndx].periph);

    // Configure PWM
    SysCtlPWMClockSet(pwmClkDiv[div]);
    
    //   Disable All Outputs
    PWMOutputState(pwm[ndx].base, 0xFF, false);

    //   Set all generator periods
    pwm[ndx].period = (reload >> div) - 1;
    PWMGenPeriodSet(pwm[ndx].base, PWM_GEN_0, pwm[ndx].period);
    PWMGenPeriodSet(pwm[ndx].base, PWM_GEN_1, pwm[ndx].period);
    PWMGenPeriodSet(pwm[ndx].base, PWM_GEN_2, pwm[ndx].period);
    PWMGenPeriodSet(pwm[ndx].base, PWM_GEN_3, pwm[ndx].period);

    //   Set all generator modes
    PWMGenConfigure(pwm[ndx].base, PWM_GEN_0, PWM_GEN_MODE_DOWN);
    PWMGenConfigure(pwm[ndx].base, PWM_GEN_1, PWM_GEN_MODE_DOWN);
    PWMGenConfigure(pwm[ndx].base, PWM_GEN_2, PWM_GEN_MODE_DOWN);
    PWMGenConfigure(pwm[ndx].base, PWM_GEN_3, PWM_GEN_MODE_DOWN);
    
    //   Enable all generators
    PWMGenEnable(pwm[ndx].base, PWM_GEN_0);
    PWMGenEnable(pwm[ndx].base, PWM_GEN_1);
    PWMGenEnable(pwm[ndx].base, PWM_GEN_2);
    PWMGenEnable(pwm[ndx].base, PWM_GEN_3);

    // Initialization completed
    pwm[ndx].isInit = true;

	return PWM_NO_ERR;
}

void PWM_Set(unsigned char ndx, unsigned char pin_ndx, float duty)
{
    if (duty > 0.99)
        duty = 0.99;
    else if (duty < 0.01)
        duty = 0.01;

    PWMPulseWidthSet(pwm[ndx].base, pwm[ndx].pins[pin_ndx].pwm_out, pwm[ndx].period * duty );
}

void PWM_Invert(unsigned char ndx, unsigned char pin_ndx, bool inv)
{
   PWMOutputInvert(pwm[ndx].base, pwm[ndx].pins[pin_ndx].out_bit, inv); 
}


void PWM_Enable(unsigned char ndx, unsigned char pin_ndx)
{
    // Too much dereferencing. Let's minimize that a bit
    tPWM_Pin pwm_pin = pwm[ndx].pins[pin_ndx];

    // Enable Pin Port
    // TODO: Initialize this better?
    IO_Init(pwm_pin.pin);
    
    // Set GPIO Pin Mux
    GPIOPinTypePWM(pins[pwm_pin.pin].port.base, pins[pwm_pin.pin].offset);
    GPIOPinConfigure(pwm_pin.pin_mux);
    
    // Enable PWM Output
    PWMOutputState(pwm[ndx].base, pwm_pin.out_bit, true);
}

void PWM_Disable(unsigned char ndx, unsigned char pin_ndx)
{
    // Too much dereferencing. Let's minimize that a bit
    tPWM_Pin pwm_pin = pwm[ndx].pins[pin_ndx];

    // Disable PWM Output
    PWMOutputState(pwm[ndx].base, pwm_pin.out_bit, false);

    // Set GPIO Pin Mux
    GPIOPinTypeGPIOInput(pins[pwm_pin.pin].port.base, pins[pwm_pin.pin].offset);
}
