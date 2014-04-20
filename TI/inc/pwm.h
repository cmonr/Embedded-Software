#include <stdbool.h>
#include <stdint.h>
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pwm.h>
#include <driverlib/pin_map.h>

#include "pin.h"


#ifndef _PWM_H_
#define _PWM_H_

// TODO: Unroll this in the future
#define PWM0 &_pwm[0]
#define PWM1 &_pwm[3]

#define PWM_MIN_RESOLUTION 100


typedef struct
{
    tPinName pin;
    unsigned int pin_mux;

    unsigned int generator;
    unsigned int pwm_out;
    unsigned int out_bit;
} tPWM_Pin;


typedef struct
{
    unsigned int periph;
    unsigned int base;

    tPWM_Pin pins[8];

    unsigned char clkDiv;
    unsigned long freq;
    unsigned long period;

    bool isInit;
} tPWM;

typedef enum
{
    PWM_NO_ERR,
    PWM_IS_INITIALIZED,
    PWM_FREQ_TOO_LOW,
    PWM_INSUFFICIENT_RESOLUTION
} tPWM_ERR;

tPWM_ERR PWM_Init( tPWM*, unsigned long );

void PWM_Set( tPWM*, unsigned char, float );
void PWM_Invert( tPWM*, unsigned char, bool );
void PWM_Enable( tPWM*, unsigned char );
void PWM_Disable( tPWM*, unsigned char );

extern tPWM _pwm[2];

extern tPin pins[PIN_COUNT];

#endif
