#define PART_TM4C123GH6PM 1

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/timer.h"
#include "driverlib/fpu.h"
#include "driverlib/systick.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"

#define period  1024
#define PI      3.141592
#define E       2.718282
#define refresh 256

static int x = 0;
void SysTickIntHandler(void)
{
    TimerMatchSet(TIMER1_BASE, TIMER_A, (unsigned int) ((period-512) * ((E - exp(cos((x & 0xFF) * PI / (refresh/2)))) / E)) + 512 + 68);

    x++;
}

int main(void)
{
    // Clock (80MHz)
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);


    // GPIO
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinConfigure(GPIO_PF2_T1CCP0);
    GPIOPinTypeTimer(GPIO_PORTF_BASE, GPIO_PIN_2);


    // Timer/PWM
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    TimerConfigure(TIMER1_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM);
    TimerLoadSet(TIMER1_BASE, TIMER_A, period - 1); // Period (cycles)
    TimerMatchSet(TIMER1_BASE, TIMER_A, period - 1);     // Duty Cycle (% on)

    TimerEnable(TIMER1_BASE, TIMER_A);

  
    // FPU
    FPUEnable();
    FPULazyStackingEnable();


    // SysTick
    SysTickPeriodSet(SysCtlClockGet() / refresh * 2);

    SysTickIntEnable();
    SysTickEnable();


    // Enable Interrupts
    IntMasterEnable();


    // Idle
    while(1);
}
