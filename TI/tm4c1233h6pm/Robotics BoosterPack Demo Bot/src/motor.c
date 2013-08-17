#define PART_TM4C1233H6PM 1

#include <stdbool.h>
#include <stdint.h>
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

volatile static bool _motor_inv[4];

void initMotors(bool inv0, bool inv1, bool inv2, bool inv3)
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
}

void enableMotors(bool enable)
{
   // Set IO Expander bit

}

void setMotor(unsigned char num, float duty)
{
    if (num > 3)
       return;

    // 0.5 = Midpoint

    //TimerMatchSet(_motor_timer_sets[a][0], _motor_timer_sets[a][1], SysCtlClockGet() / 10000 * b)
}
