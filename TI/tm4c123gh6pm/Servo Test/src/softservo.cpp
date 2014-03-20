#include <stdint.h>
#include <inc/hw_memmap.h>
#include <inc/hw_gpio.h>
#include <inc/hw_ints.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/timer.h>
#include <driverlib/interrupt.h>
#include <driverlib/pin_map.h>

#include "softservo.h"

SoftServo servo6(GPIO_PORTD_BASE, GPIO_PIN_2),
          servo7(GPIO_PORTD_BASE, GPIO_PIN_3),
          servo8(GPIO_PORTE_BASE, GPIO_PIN_0),
          servo9(GPIO_PORTE_BASE, GPIO_PIN_1),
          servo10(GPIO_PORTE_BASE, GPIO_PIN_2),
          servo11(GPIO_PORTE_BASE, GPIO_PIN_3);

SoftServo dummySoftServo(0, 0);

SoftServo::SoftServo(unsigned int port, unsigned int pin)
{
    _port = port;
    _pin = pin;
}

void SoftServo::set(float duty)
{
    // Bounds check
    if (duty < 0)
        duty = 0.0;
    else if (duty > 1.0)
        duty = 1.0;
    
    // Check for inversion
    if (_invert)
        duty = 1.0 - duty;

    // Calculate modified duty cycle
    duty = _offset + duty * _range;
    
    // Set Timer On Duration
    _duration = _pwm_period * duty;
    
}

void SoftServo::setLimits(float min, float max)
{
    // Disable servo while configuring
    enable(false);

    // Calculate internal variables
    _offset = min;
    _range = max-min;
    
    // Reset to neutral position 
    set(0.5);
}

void SoftServo::invert()
{
    // Disable servo while configuring
    enable(false);

    // Invert servo direction
    _invert = !_invert;

    // Reset to neutral position 
    set(0.5);
}

void SoftServo::enable(bool enable)
{
    if (enable == true)
    {
        // Enable IO
        GPIOPinTypeGPIOOutput(_port, _pin);
        GPIOPinWrite(_port, _pin, 0);
    }else{
        // Disable IO
        GPIOPinTypeGPIOInput(_port, _pin);
    }

    // IO will be updated by Timer ISR
}



void initSoftServos()
{
    unsigned int pwm_period;  
    
    // Only use WTIMER5 for now

    //  Enable Peripherals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER5);

    // Configure WTimer
    TimerConfigure(WTIMER5_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC | TIMER_CFG_B_PERIODIC);
    pwm_period = SysCtlClockGet() / 50 / 8;
    TimerLoadSet(WTIMER5_BASE, TIMER_A, pwm_period); // Mux 8 Servos
    TimerLoadSet(WTIMER5_BASE, TIMER_B, pwm_period / 2);
   
    TimerIntEnable(WTIMER5_BASE, TIMER_TIMA_TIMEOUT | TIMER_TIMB_TIMEOUT);
    TimerEnable(WTIMER5_BASE, TIMER_BOTH);

    IntEnable(INT_WTIMER5A);
    IntEnable(INT_WTIMER5B);

    
    // Finish servo initializations
    servo6._pwm_period = pwm_period;
    servo7._pwm_period = pwm_period;
    servo8._pwm_period = pwm_period;
    servo9._pwm_period = pwm_period;
    servo10._pwm_period = pwm_period;
    servo11._pwm_period = pwm_period;

    servo6.set(0.5);
    servo7.set(0.5);
    servo8.set(0.5);
    servo9.set(0.5);
    servo10.set(0.5);
    servo11.set(0.5);
}


volatile SoftServo* _softServo[8] = {
    &servo6,
    &servo7,
    &servo8,
    &servo9,
    &servo10,
    &servo11,
    &dummySoftServo,
    &dummySoftServo
};

volatile unsigned char _softServoNdx = 0;

extern "C" void WTimer5AIntHandler(void)
{
    // Move to next Servo Pin
    _softServoNdx = (_softServoNdx + 1) & 0x7;
   
    // Enable Servo Pin
    GPIOPinWrite(_softServo[_softServoNdx]->_port, _softServo[_softServoNdx]->_pin, 0xFF);

    // Set new servo value
    TimerDisable(WTIMER5_BASE, TIMER_B);
    TimerLoadSet(WTIMER5_BASE, TIMER_B, _softServo[_softServoNdx]->_duration);
    TimerEnable(WTIMER5_BASE, TIMER_B);

    // Ack Interrupt
    TimerIntClear(WTIMER5_BASE, TIMER_TIMA_TIMEOUT);        
}

extern "C" void WTimer5BIntHandler(void)
{
    // Bring current Servo Pin low
    GPIOPinWrite(_softServo[_softServoNdx]->_port, _softServo[_softServoNdx]->_pin, 0);
    
    // Ack Interrupt
    TimerIntClear(WTIMER5_BASE, TIMER_TIMB_TIMEOUT);
}
