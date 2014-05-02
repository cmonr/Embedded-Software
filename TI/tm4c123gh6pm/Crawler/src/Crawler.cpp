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
#include "Leg.h"

Crawler::Crawler()
: NUM_SERVOS(sizeof(servos)/sizeof(SoftServo)), stepDelay(0.25)
{
    initServos();

    hservos[0] = Servo(PWM_OUT_0, PWM_OUT_0_BIT);
    hservos[1] = Servo(PWM_OUT_1, PWM_OUT_1_BIT);
    hservos[2] = Servo(PWM_OUT_2, PWM_OUT_2_BIT);
    hservos[3] = Servo(PWM_OUT_3, PWM_OUT_3_BIT);

    // Cascade Servo Initalization
    //  Limit sudden power draw
    for (int i = 0; i < NUM_SERVOS; i++) {
        hservos[i].enable();
        delay(0.25);
    }
    
    // Initialize Soft Servos
    initSoftServos();

    servos[0] = SoftServo(GPIO_PORTD_BASE, GPIO_PIN_2);
    servos[1] = SoftServo(GPIO_PORTE_BASE, GPIO_PIN_0);
    servos[2] = SoftServo(GPIO_PORTD_BASE, GPIO_PIN_3);
    servos[3] = SoftServo(GPIO_PORTE_BASE, GPIO_PIN_1);
    
    // Attach SoftServo Objects to SoftServo Generator
    for (int i = 0; i < NUM_SERVOS; i++) {
    //for (int i = 2; i < 4; i++) {
        attachSoftServo(&servos[i]);
    }   

    // Cascase Servo Initalization
    for (int i = 0; i < NUM_SERVOS; i++) {
    //for (int i = 2; i < 4; i++) {
        servos[i].enable();
        delay(0.25);
    }
}

void Crawler::crawlForward(void) {
    Leg frontLeft(&servos[1], &servos[0], 2);
    frontLeft.setKSteps(.5, 1, 1, .5);
    frontLeft.setHSteps(0, 0, 1, 1);
    
    Leg frontRight(&servos[3], &servos[2]);
    frontRight.setKSteps(.5, 1, 1, .5);
    frontRight.setHSteps(1, 1, 0, 0);
    
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 4; j++) {
            hservos[0].set(i/10.0);
        }
        frontLeft.step();
        frontRight.step();
        delay(stepDelay);
    }
    
    for (int i = 0; i < NUM_SERVOS; i++) {
        servos[i].disable();
        hservos[i].disable();
    }
}

