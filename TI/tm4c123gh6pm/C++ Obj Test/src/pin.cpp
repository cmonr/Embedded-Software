class Pin
{
    public:
        Pin( unsigned char );

        unsigned char &offset;
        tPort &port;

    private:
        unsigned char _offset;
        tPort _port;
}


// Declare all pins here

typedef struct
{
    unsigned int periph;
    unsigned int base;
}tPort;


tPort port[6] = 
{
    {SYSCTL_PERIPH_GPIOA, GPIO_PORTA_BASE},
    {SYSCTL_PERIPH_GPIOB, GPIO_PORTB_BASE},
    {SYSCTL_PERIPH_GPIOC, GPIO_PORTC_BASE},
    {SYSCTL_PERIPH_GPIOD, GPIO_PORTD_BASE},
    {SYSCTL_PERIPH_GPIOE, GPIO_PORTE_BASE},
    {SYSCTL_PERIPH_GPIOF, GPIO_PORTF_BASE}
};
