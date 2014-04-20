#define PART_TM4C1233H6PM 1

#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <inc/hw_memmap.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/timer.h>

#include "motor.h"


static const uint32_t _motor_timer_sets[4][2]=
{
  {WTIMER0_BASE, TIMER_A},
  {WTIMER0_BASE, TIMER_B},
  {WTIMER1_BASE, TIMER_A},
  {WTIMER1_BASE, TIMER_B}
};

static const uint32_t _motor_gpio_sets[4][2]=
{
  {GPIO_PORTD_BASE, GPIO_PIN_0},
  {GPIO_PORTD_BASE, GPIO_PIN_1},
  {GPIO_PORTA_BASE, GPIO_PIN_6},
  {GPIO_PORTA_BASE, GPIO_PIN_7}
};

volatile static bool _motor_inv[4] = {false};
static float pwm_period;

void initMotors()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1);
   

    // PWM (Motors)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER1);
    GPIOPinConfigure(GPIO_PC4_WT0CCP0);
    GPIOPinConfigure(GPIO_PC5_WT0CCP1);
    GPIOPinConfigure(GPIO_PC6_WT1CCP0);
    GPIOPinConfigure(GPIO_PC7_WT1CCP1);
    GPIOPinTypeTimer(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);

    TimerConfigure(WTIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM | TIMER_CFG_B_PWM);
    TimerLoadSet(WTIMER0_BASE, TIMER_BOTH, SysCtlClockGet() / 10000); //10KHz
    TimerMatchSet(WTIMER0_BASE, TIMER_A, 1);
    TimerMatchSet(WTIMER0_BASE, TIMER_B, 1);

    TimerConfigure(WTIMER1_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM | TIMER_CFG_B_PWM);
    TimerLoadSet(WTIMER1_BASE, TIMER_BOTH, SysCtlClockGet() / 10000); //10KHz
    TimerMatchSet(WTIMER1_BASE, TIMER_A, 1);
    TimerMatchSet(WTIMER1_BASE, TIMER_B, 1);

    TimerEnable(WTIMER0_BASE, TIMER_A);
    TimerEnable(WTIMER0_BASE, TIMER_B);
    TimerEnable(WTIMER1_BASE, TIMER_A);
    TimerEnable(WTIMER1_BASE, TIMER_B);


    // Motors Fwd
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_6 | GPIO_PIN_7);
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_0 | GPIO_PIN_1);

    
    // Generate PWM Value
    pwm_period = ((float) SysCtlClockGet()) / 10000 - 2;
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
    normalized_duty = 1-fabs(duty);

    // Do some magic!
    GPIOPinWrite(_motor_gpio_sets[num][0], _motor_gpio_sets[num][1], duty > 0.0 ? 0xFF : 0x00);
    TimerMatchSet(_motor_timer_sets[num][0], _motor_timer_sets[num][1], ((unsigned long) pwm_period * normalized_duty) + 1);
}

