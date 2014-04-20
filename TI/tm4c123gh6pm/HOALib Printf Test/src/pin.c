#include "pin.h"

tPort ports[6] =
{
    {SYSCTL_PERIPH_GPIOA, GPIO_PORTA_BASE},
    {SYSCTL_PERIPH_GPIOB, GPIO_PORTB_BASE},
    {SYSCTL_PERIPH_GPIOC, GPIO_PORTC_BASE},
    {SYSCTL_PERIPH_GPIOD, GPIO_PORTD_BASE},
    {SYSCTL_PERIPH_GPIOE, GPIO_PORTE_BASE},
    {SYSCTL_PERIPH_GPIOF, GPIO_PORTF_BASE}
};

tPin pins[PIN_COUNT];

tPinName Pin_Init(tPinName pin)
{
    // Check if object already exists
    if (pins[pin].isInit == true)
        return ERR;

    // Figure out internal variables
    pins[pin].port = ports[pin >> 3];
    pins[pin].offset = 1 << (pin & 0x07);

    // Default Pin Values
    pins[pin].state = HiZ;

    // Enable Port
    SysCtlPeripheralEnable(pins[pin].port.periph);

    // Pin initialized
    pins[pin].isInit = true;

    return pin;
}

void Pin_Set(tPinName pin, tPin_State state)
{
    // Set Pin
    if (pins[pin].state == HiZ)
        GPIOPinTypeGPIOInput(pins[pin].port.base, pins[pin].offset);
    else
    {
        GPIOPinTypeGPIOOutput(pins[pin].port.base, pins[pin].offset);

        if (state == LOW)
            GPIOPinWrite(pins[pin].port.base, pins[pin].offset, 0);
        else
            GPIOPinWrite(pins[pin].port.base, pins[pin].offset, pins[pin].offset);
    }
    
    // Update internal variable
    pins[pin].state = state;
}

void Pin_Toggle(tPinName pin)
{
    // Pin is an input
    if (pins[pin].state == HiZ)
        return;
    
    // Toggle Pin
    if (pins[pin].state == LOW)
        Pin_Set(pin, HIGH);
    else
        Pin_Set(pin, LOW);
}

unsigned char Pin_Read(tPinName pin)
{
    // Return 1 or 0
    return GPIOPinRead(pins[pin].port.base, pins[pin].offset) == 0 ? 0 : 1;
}

void Pin_Enable(tPinName pin)
{
    Pin_Set(pin, LOW);
}

void Pin_Disable(tPinName pin)
{
    Pin_Set(pin, HiZ);
}
