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

#include "Crawler.h"
#include "LegKinds.h"

Crawler::Crawler()
: NUM_SERVOS(sizeof(servos)/sizeof(SoftServo)) 
{
    // Initialize Soft Servos
    initSoftServos();

    servos[0] = SoftServo(GPIO_PORTD_BASE, GPIO_PIN_2);
    servos[1] = SoftServo(GPIO_PORTD_BASE, GPIO_PIN_3);
    servos[2] = SoftServo(GPIO_PORTE_BASE, GPIO_PIN_0);
    servos[3] = SoftServo(GPIO_PORTE_BASE, GPIO_PIN_1);
    
    // Attach SoftServo Objects to SoftServo Generator
    for (int i = 0; i < NUM_SERVOS; i++) {
        attachSoftServo(&servos[i]);
    }   

    // Cascase Servo Initalization
    for (int i = 0; i < NUM_SERVOS; i++) {
        servos[i].enable();
        delay(0.25);
    }
}

void Crawler::crawlForward(void) {
    LeftLeg frontLeft(&servos[0], &servos[2]);
    RightLeg frontRight(&servos[1], &servos[3]);
    
    for (int i = 0; i < 40; i++) {
        frontLeft.step();
        frontRight.step();
        delay(1.0);
    }
    
    for (int i = 0; i < NUM_SERVOS; i++) {
        servos[i].disable();
    }
}

