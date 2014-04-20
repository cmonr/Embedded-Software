#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <driverlib/gpio.h>
#include <driverlib/uart.h>
#include <driverlib/i2c.h>
#include <driverlib/pwm.h>
#include <driverlib/pin_map.h>
#include <driverlib/interrupt.h>
#include <driverlib/sysctl.h>

#include "led.h"
#include "servo.h"
#include "softservo.h"

#define delay(x)      SysCtlDelay(SysCtlClockGet() / 3 * x);

void init(void) {
    // Clock (80MHz)
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // GPIO
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0); 


    // UART (Andrtoid Serial)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE | UART_CONFIG_WLEN_8));
    
    UARTEnable(UART0_BASE);
    UARTIntEnable(UART0_BASE, UART_INT_RX);
    IntEnable(INT_UART0);
}

int main(void)
{
    // Initialize LEDs
    initLEDs();
    
    LED rLED = LED(PWM_OUT_5, PWM_OUT_5_BIT);
    rLED.enable();

    // Initialize Servos
    initServos();

    const int NUM_SERVOS = 2;
    Servo servos[NUM_SERVOS] = {
        Servo(PWM_OUT_0, PWM_OUT_0_BIT),
        Servo(PWM_OUT_1, PWM_OUT_1_BIT)
        };

    // Cascade Servo Initalization
    //  Limit sudden power draw
    for (int i = 0; i < NUM_SERVOS; i++) {
        servos[i].enable();
        delay(0.25);
    }
    
    // Enable Interrupts
    IntMasterEnable();

    while (true) {
        printf("hi\n");

        delay(1);
        rLED.set(1.0);
        
        for (int i = 0; i < NUM_SERVOS; i++) {
            servos[i].set(0.6);
        }
    
        delay(1);
        rLED.set(0);
        
        for (int i = 0; i < NUM_SERVOS; i++) {
            servos[i].set(0.4);
        }
    }
}
