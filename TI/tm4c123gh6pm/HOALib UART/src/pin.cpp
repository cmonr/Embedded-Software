//#include <stdbool.h>
//#include <stdint.h>
//#include <inc/hw_memmap.h>
//#include <driverlib/sysctl.h>
//#include <driverlib/gpio.h>

#include "pin.h"

tPort _ports[6] =
{
    {SYSCTL_PERIPH_GPIOA, GPIO_PORTA_BASE},
    {SYSCTL_PERIPH_GPIOB, GPIO_PORTB_BASE},
    {SYSCTL_PERIPH_GPIOC, GPIO_PORTC_BASE},
    {SYSCTL_PERIPH_GPIOD, GPIO_PORTD_BASE},
    {SYSCTL_PERIPH_GPIOE, GPIO_PORTE_BASE},
    {SYSCTL_PERIPH_GPIOF, GPIO_PORTF_BASE}
};

Pin* pins[PIN_COUNT];


Pin::Pin(tPin pin)
{
    // TODO: Add check for previous allocation
    //   For now, assume user is competent
    pins[pin] = this;

    // Figure out internal variables
    offset = 1 << (pin & 0x07);
    port = _ports[pin >> 3];

    // Default Pin Values
    _dir = IN;
    _outState = LOW;

    // Enable Port
    SysCtlPeripheralEnable(port.periph);
}

void Pin::setDir(PinDir dir)
{
    // Set direction
    if (dir == IN)
        GPIOPinTypeGPIOInput(port.base, offset);
    else
        GPIOPinTypeGPIOOutput(port.base, offset);

    // Save internal variable
    _dir = dir;
}

void Pin::set(PinOutState state)
{
    // Set Pin
    if (state == LOW)
        GPIOPinWrite(port.base, offset, 0);
    else
        GPIOPinWrite(port.base, offset, offset);
    
    // Update internal variable
    _outState = state;
}

void Pin::toggle()
{
    if (_outState == LOW)
        set(HIGH);
    else
        set(LOW);
}

unsigned char Pin::read()
{
    // Return 1 or 0
    return GPIOPinRead(port.base, offset) != 0 ? 1 : 0;
}

void Pin::enable()
{
    // This is silly, but essentially set IO again 
    setDir(_dir);
}

void Pin::disable()
{
    // This is silly, but make Pin an input
    GPIOPinTypeGPIOInput(port.base, offset);
}
