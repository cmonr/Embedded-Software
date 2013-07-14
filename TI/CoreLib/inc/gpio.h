#include <stdint>
#include <stdbool>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"


#define GPIO_PORTA_BASE  PortA
#define GPIO_PORTB_BASE  PortB
#define GPIO_PORTC_BASE  PortC
#define GPIO_PORTD_BASE  PortD
#define GPIO_PORTE_BASE  PortE
#define GPIO_PORTF_BASE  PortF


#define GPIO_PIN_0  P0
#define GPIO_PIN_1  P1
#define GPIO_PIN_2  P2
#define GPIO_PIN_3  P3
#define GPIO_PIN_4  P4
#define GPIO_PIN_5  P5
#define GPIO_PIN_6  P6
#define GPIO_PIN_7  P7



typedef struct
{
  uint32_t port;
  uint32_t num;
} tPin;

typedef enum
{
  LOW = 0,
  HIGH = 1
} tPinState, tPortState;

typedef enum
{
  PUSHPULL = GPIO_PIN_TYPE_STD,
  PUSHPULL_PULLUP = GPIO_PIN_TYPE_WPU,
  PUSHPULL_PULLDOWN = GPIO_PIN_TYPE_WPD,
  OPENCOLLECTOR = GPIO_PIN_TYPE_OD,
  ANALOG = GPIO_PIN_TYPE_ANALOG
} tPinMode, tPortMode;

typedef enum
{
  TWO_MA = GPIO_STRENGTH_2MA,
  FOUR_MA = GPIO_STRENGTH_4MA,
  EIGHT_MA = GPIO_STRENGTH_8MA
} tPinStrength, tPortStrength;

typedef enum
{
  FALLING = GPIO_FALLING_EDGE,
  RISING = GPIO_RISING_EDGE,
  BOTH = GPIO_BOTH_EDGES
} tPortIntType;



tPin A[8] = {
  { PortA, P0 },
  { PortA, P1 },
  { PortA, P2 },
  { PortA, P3 },
  { PortA, P4 },
  { PortA, P5 },
  { PortA, P6 },
  { PortA, P7 }
};

tPin B[8] = {
  { PortB, P0 },
  { PortB, P1 },
  { PortB, P2 },
  { PortB, P3 },
  { PortB, P4 },
  { PortB, P5 },
  { PortB, P6 },
  { PortB, P7 }
};

tPin C[8] = {
  { PortC, P0 },
  { PortC, P1 },
  { PortC, P2 },
  { PortC, P3 },
  { PortC, P4 },
  { PortC, P5 },
  { PortC, P6 },
  { PortC, P7 }
};

tPin D[8] = {
  { PortD, P0 },
  { PortD, P1 },
  { PortD, P2 },
  { PortD, P3 },
  { PortD, P4 },
  { PortD, P5 },
  { PortD, P6 },
  { PortD, P7 }
};

tPin E[8] = {
  { PortE, P0 },
  { PortE, P1 },
  { PortE, P2 },
  { PortE, P3 },
  { PortE, P4 },
  { PortE, P5 },
  { PortE, P6 },
  { PortE, P7 }
};

tPin F[8] = {
  { PortF, P0 },
  { PortF, P1 },
  { PortF, P2 },
  { PortF, P3 },
  { PortF, P4 },
  { PortF, P5 },
  { PortF, P6 },
  { PortF, P7 }
};



void initPin( tPin pin );
void setPinInput( tPin pin );
void setPinOutput( tPin pin, tPinState state );

void setPinConfig( tPin pin, tPinMode mode, tPinStrength, strength);

void writePin( tPin pin, tPinState state );
void togglePin( tPin pin );
unsigned char readPin( tPin pin );


void initPort( uint32_t port, uint32_t pins );
void setPortInput( uint32_t port, uint32_t pins );
void setPortOutput( uint32_t port, uint32_t pins, tPortState state );

void setPinConfig( uint32_t port, tPortMode mode, tPortStrength strength );

void writePort( uint32_t port, uint32_t pins, tPortState state );
void togglePort( uint32_t port, uint32_t pins );
unsigned char readPort( uint32_t port, uint32_t pins );
void setPortInterrupt( uint32_t port, uint32_t pins, tPortIntType intType );


void GPIOPortAIntHandler( void );
void GPIOPortBIntHandler( void );
void GPIOPortCIntHandler( void );
void GPIOPortDIntHandler( void );
void GPIOPortEIntHandler( void );
void GPIOPortFIntHandler( void );
