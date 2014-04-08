#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>

#define PORT_WIDTH 8
#define PORT_COUNT PIN_COUNT / PORT_WIDTH

typedef enum {
    PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7,
    PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7,
    PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7,
    PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7,
    PE0, PE1, PE2, PE3, PE4, PE5, PE6, PE7,
    PF0, PF1, PF2, PF3, PF4, PF5, PF6, PF7,
    PIN_COUNT
} tPin;

class Pin
{
    public:
        typedef enum { HiZ, Low, High } PinDir;
        typedef enum { OpenCollector, PushPull } PinMode;

        Pin( unsigned char ): _offset(offset), _port(port);

        void setDir( enum PinDir );
        void toggle( void );
        unsigned char read( void );

        unsigned char &offset;
        tPort &port;

    private:
        unsigned char _offset;
        tPort _port;
}

Pin* _pin[PIN_COUNT] = {0};



typedef struct
{
    unsigned int periph;
    unsigned int base;
}tPort;


tPort _port[6] = 
{
    {SYSCTL_PERIPH_GPIOA, GPIO_PORTA_BASE},
    {SYSCTL_PERIPH_GPIOB, GPIO_PORTB_BASE},
    {SYSCTL_PERIPH_GPIOC, GPIO_PORTC_BASE},
    {SYSCTL_PERIPH_GPIOD, GPIO_PORTD_BASE},
    {SYSCTL_PERIPH_GPIOE, GPIO_PORTE_BASE},
    {SYSCTL_PERIPH_GPIOF, GPIO_PORTF_BASE}
};

/* Example use case: 
Pin btn = Pin(PF0);  // Initialization unlucks PF0 and populates _pin[PF0] entry
...
btn.setDir(In);
while(btn.read() == Pin.LOW);
while(btn.read() == Pin.HIGH);
// Button Pressed. Do stuff.

*/
