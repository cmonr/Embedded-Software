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
#include "Crawler.h"

int main(void) {
    // Clock (80MHz)
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    
    // Initialize LEDs
    initLEDs();
    LED rLED = LED(PWM_OUT_5, PWM_OUT_5_BIT);
    rLED.enable();
  
    Crawler crawler;

    // Enable Interrupts
    IntMasterEnable();

    crawler.crawlForward();
    
    while (true) {}
}
