#define PART_TM4C123GH6PM 1

#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <inc/hw_gpio.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pwm.h>
#include <driverlib/pin_map.h>
#include <driverlib/timer.h>

#include "motor.h"


static const uint32_t _motor_pwm_sets[4][3]=
{
  {PWM1_BASE, PWM_OUT_3, PWM_OUT_3_BIT},
  {PWM1_BASE, PWM_OUT_2, PWM_OUT_2_BIT},
  {PWM1_BASE, PWM_OUT_1, PWM_OUT_1_BIT},
  {PWM1_BASE, PWM_OUT_0, PWM_OUT_0_BIT}
};

static const uint32_t _motor_gpio_sets[4][2]=
{
  {GPIO_PORTF_BASE, GPIO_PIN_0},
  {GPIO_PORTF_BASE, GPIO_PIN_4},
  {GPIO_PORTC_BASE, GPIO_PIN_4},
  {GPIO_PORTC_BASE, GPIO_PIN_7}
};

volatile static bool _motor_inv[4] = {false};
static int pwm_period;

void initMotors()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);


    // Setup IO
    // PWM
    GPIOPinTypePWM(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7);
    GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    GPIOPinConfigure(GPIO_PD0_M1PWM0);
    GPIOPinConfigure(GPIO_PD1_M1PWM1);
    GPIOPinConfigure(GPIO_PA6_M1PWM2);
    GPIOPinConfigure(GPIO_PA7_M1PWM3);
    
    
    // Unlock F0 Pin (NMI)
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR)  |= GPIO_PIN_0;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;


    // GPIO
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_7);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_PIN_0 | GPIO_PIN_4);
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_7, GPIO_PIN_4 | GPIO_PIN_7);


    // Configure PWM
    SysCtlPWMClockSet(SYSCTL_PWMDIV_64); // PWM Clock Divider
    pwm_period = SysCtlClockGet() / 64 / 25000 - 1; // 25KHz

    PWMGenConfigure(PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN);
    PWMGenConfigure(PWM1_BASE, PWM_GEN_1, PWM_GEN_MODE_DOWN);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, pwm_period);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_1, pwm_period);


    // Initialize PWM Duty Cycles
    setMotor(0, 0.0);
    setMotor(1, 0.0);
    setMotor(2, 0.0);
    setMotor(3, 0.0);


    // Enable PWM
    PWMGenEnable(PWM1_BASE, PWM_GEN_0);
    PWMGenEnable(PWM1_BASE, PWM_GEN_1);

    
    // Enable Pin Outputs
    PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT, true);
    PWMOutputState(PWM1_BASE, PWM_OUT_1_BIT, true);
    PWMOutputState(PWM1_BASE, PWM_OUT_2_BIT, true);
    PWMOutputState(PWM1_BASE, PWM_OUT_3_BIT, true);
}

void invertMotor(unsigned char num)
{
    if (num > 3)
        return;

    _motor_inv[num] = !(_motor_inv[num]);
}

void enableMotors(bool enable)
{
    // Set IO Expander bit

}

void setMotor(unsigned char num, float duty)
{
    float normalized_duty;
  
    if (num > 3 || fabs(duty) > 1.0)
       return;

    // Invert motor
    if (_motor_inv[num])
        duty = 1.0 - duty;

    // Normalize the desired duty with an equation
    //normalized_duty = 1 - fabs(duty * -2 + 1);
    //normalized_duty = 1-fabs(duty);
    normalized_duty = duty / 2.0 + 0.5;
    normalized_duty = normalized_duty * 0.9 + 0.05;


    // Set duty cycles
    PWMPulseWidthSet(_motor_pwm_sets[num][0], _motor_pwm_sets[num][1], pwm_period * normalized_duty);
    //GPIOPinWrite(_motor_gpio_sets[num][0], _motor_gpio_sets[num][1], normalized_duty > 0.0 ? 0xFF : 0x00);
}

