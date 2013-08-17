#include <stdbool.h>
#include <stdint.h>
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/timer.h>
#include <driverlib/interrupt.h>

#include "servo.h"


typedef struct {
    unsigned long port;
    unsigned long pin;
    float duty;
} tServo;

volatile tServo _servos[8] = {
    {GPIO_PORTB_BASE, GPIO_PIN_6, 0.5},
    {GPIO_PORTB_BASE, GPIO_PIN_7, 0.5},
    {GPIO_PORTB_BASE, GPIO_PIN_4, 0.5},
    {GPIO_PORTB_BASE, GPIO_PIN_5, 0.5},
    {GPIO_PORTE_BASE, GPIO_PIN_4, 0.5},
    {GPIO_PORTE_BASE, GPIO_PIN_5, 0.5},
    {0, 0, 0.0},
    {0, 0, 0.0}
};

volatile unsigned char _servoNdx = 0;


void initServos(void)
{
    //  Enable Servo Pins
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, 0);
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5, 0);

    //  Eight (8) Servos per timer
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER5);
    TimerConfigure(WTIMER5_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC | TIMER_CFG_B_PERIODIC);
    TimerLoadSet(WTIMER5_BASE, TIMER_A, SysCtlClockGet() / 50 / 8); // Mux 8 Servos
    TimerLoadSet(WTIMER5_BASE, TIMER_B, SysCtlClockGet() / 50 / 16);
   
    TimerIntEnable(WTIMER5_BASE, TIMER_TIMA_TIMEOUT | TIMER_TIMB_TIMEOUT);
    TimerEnable(WTIMER5_BASE, TIMER_BOTH);

    IntEnable(INT_WTIMER5A);
    IntEnable(INT_WTIMER5B);
}

void setServo(unsigned char num, float duty)
{
    if (num > 5)
        return;

    _servos[num].duty = duty;
}

void WTimer5AIntHandler(void)
{
    // Move to next Servo Pin
    _servoNdx = (_servoNdx + 1) & 0x7;
   
    // Enable Servo Pin
    GPIOPinWrite(_servos[_servoNdx].port, _servos[_servoNdx].pin, 0xFF);

    // Set new servo value
    TimerDisable(WTIMER5_BASE, TIMER_B);
    TimerLoadSet(WTIMER5_BASE, TIMER_B, SysCtlClockGet() / 50 / 8 * _servos[_servoNdx].duty);
    TimerEnable(WTIMER5_BASE, TIMER_B);


    // Ack Interrupt
    TimerIntClear(WTIMER5_BASE, TIMER_TIMA_TIMEOUT);        
}

void WTimer5BIntHandler(void)
{
    // Bring current Servo Pin low
    GPIOPinWrite(_servos[_servoNdx].port, _servos[_servoNdx].pin, 0);
    
    // Ack Interrupt
    TimerIntClear(WTIMER5_BASE, TIMER_TIMB_TIMEOUT);
}
