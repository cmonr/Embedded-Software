#ifndef _I2C_H_
#define _I2C_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/i2c.h>
#include <driverlib/pin_map.h>

#include "pin.h"


#define I2C0 &_i2c[0]
#define I2C1 &_i2c[1]
#define I2C2 &_i2c[2]
#define I2C3 &_i2c[3]

/*typedef enum
{
    I2C_IRQ_FLAGS
} tI2C_IRQ_Flag;
*/

typedef enum
{
    I2C_MODE_STD,
    I2C_MODE_FAST,
    I2C_MODE_FAST_PLUS,
    I2C_MODE_HIGH_SPEED,
    I2C_MODES
} tI2C_Mode;

typedef enum
{
    I2C_NO_ERR,
    I2C_IS_INITIALIZED

} tI2C_Err;

typedef struct
{
    unsigned long periph;
    unsigned long base;

    tPinName sda_pin;
    tPinName scl_pin;
    unsigned int sda_pin_mux;
    unsigned int scl_pin_mux;

    // Not going to mess with I2C Interrupts for now...
    /*unsigned int nvic_ndx;
    unsigned long irq_flags;
    void (*irq[UART_IRQ_FLAGS])( void );*/

    tI2C_Mode mode;

    bool isInit;
} tI2C;

extern tI2C _i2c[4];



tI2C_Err I2C_Init( tI2C* );
void I2C_SetMode( tI2C*, tI2C_Mode );

bool I2C_Write( tI2C*, unsigned char, unsigned char);
bool I2C_WriteBuff( tI2C*, unsigned char, unsigned char*, unsigned int );

bool I2C_Read( tI2C*, unsigned char, unsigned char* );
bool I2C_ReadBuff( tI2C*, unsigned char, unsigned char*, unsigned int );


void I2C_Enable( tI2C* );
void I2C_Disable( tI2C* );


#ifdef __cplusplus
}
#endif

#endif
