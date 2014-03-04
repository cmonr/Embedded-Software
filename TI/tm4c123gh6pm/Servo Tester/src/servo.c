#define PART_TM4C123GH6PM 1

#include <stdbool.h>
#include <stdint.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h> 
#include <inc/hw_gpio.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pwm.h>
#include <driverlib/pin_map.h>

#include "servo.h"


//    setServoLimits(, 0.26, 0.96); // 6001HB
//    setServoLimits(, 0.18, 0.96); // HD-1501MG
//    setServoLimits(, 0.33, 0.84); // HD-1801MG
//    setServoLimits(, 0.32, 0.94); // HS-82MG
//    setServoLimits(, 0.32, 0.94); // TGY1501

typedef struct
{
    unsigned long pwm;
    unsigned long bit;
    float offset;
    float range;
    bool enable;
    bool invert;
    float duty;
} tServo;

volatile tServo _servos[6] = 
{
    {PWM_OUT_0, PWM_OUT_0_BIT, 0.0, 1.0, false, false, 0.5},
    {PWM_OUT_1, PWM_OUT_1_BIT, 0.0, 1.0, false, false, 0.5},
    {PWM_OUT_2, PWM_OUT_2_BIT, 0.0, 1.0, false, false, 0.5},
    {PWM_OUT_3, PWM_OUT_3_BIT, 0.0, 1.0, false, false, 0.5},
    {PWM_OUT_4, PWM_OUT_4_BIT, 0.0, 1.0, false, false, 0.5},
    {PWM_OUT_5, PWM_OUT_5_BIT, 0.0, 1.0, false, false, 0.5}
};

static int pwm_period;

void enableServos()
{
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
    
    
    // Initialize PWM Duty Cycles
    setServo(0, 0.5);
    setServo(1, 0.5);
    setServo(2, 0.5);
    setServo(3, 0.5);
    setServo(4, 0.5);
    setServo(5, 0.5);

    
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
}


void setServo(unsigned char num, float duty)
{
    
    if (num > 5 || duty < 0.0 || duty > 1.0)
        return;

    if (_servos[num].invert)
        duty = 1.0 - duty;

    _servos[num].duty = _servos[num].offset + (duty * _servos[num].range);

    PWMPulseWidthSet(PWM0_BASE, _servos[num].pwm, _servos[num].duty * pwm_period / 8.0);
}


void setServoLimits(unsigned char num, float min, float max)
{
    if (num > 5 || min > max || min < 0.0 || max > 1.0)
        return;

    disableServo(num);

    _servos[num].offset = min;
    _servos[num].range = max - min;
}


void invertServo(unsigned char num, bool invert)
{
    if (num > 5)
        return;

    disableServo(num);

    _servos[num].invert = invert;
}


void enableServo(unsigned char num)
{
    PWMOutputState(PWM0_BASE, _servos[num].bit, true);
}


void disableServo(unsigned char num)
{
    PWMOutputState(PWM0_BASE, _servos[num].bit, false);
}
