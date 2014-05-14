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

Crawler::Crawler(float stepDelay)
: NUM_SERVOS(sizeof(sservos)/sizeof(SoftServo)), stepDelay(stepDelay)
{
    // Initialize hardware servos
    initHardServos();

    hservos[0] = HardServo(PWM_OUT_0, PWM_OUT_0_BIT);
    hservos[1] = HardServo(PWM_OUT_1, PWM_OUT_1_BIT);
    hservos[2] = HardServo(PWM_OUT_2, PWM_OUT_2_BIT);
    hservos[3] = HardServo(PWM_OUT_3, PWM_OUT_3_BIT);

    // Cascade Servo Initalization
    //  Limit sudden power draw
    for (int i = 0; i < NUM_SERVOS; i++) {
        hservos[i].enable();
        delay(0.25);
    }
    
    // Initialize Soft Servos
    initSoftServos();

    sservos[0] = SoftServo(GPIO_PORTD_BASE, GPIO_PIN_2);
    sservos[1] = SoftServo(GPIO_PORTE_BASE, GPIO_PIN_0);
    sservos[2] = SoftServo(GPIO_PORTD_BASE, GPIO_PIN_3);
    sservos[3] = SoftServo(GPIO_PORTE_BASE, GPIO_PIN_1);
    
    // Attach SoftServo Objects to SoftServo Generator
    for (int i = 0; i < NUM_SERVOS; i++) {
        attachSoftServo(&sservos[i]);
    }   

    // Cascase Servo Initalization
    for (int i = 0; i < NUM_SERVOS; i++) {
        sservos[i].enable();
        delay(0.25);
    }
}

void Crawler::crawlForward(void) {
    Leg frontLeft(&sservos[1], &sservos[0], 3, false);
    frontLeft.setKSteps(.5, 1, 1, .5);
    frontLeft.setHSteps(1, 1, .5, .5);
    
    Leg frontRight(&sservos[3], &sservos[2], 1, false);
    frontRight.setKSteps(.5, 1, 1, .5);
    frontRight.setHSteps(0, 0, .5, .5);
    
    Leg backLeft(&hservos[0], &hservos[1], 1, false);
    backLeft.setKSteps(.5, 0, 0, .5);
    backLeft.setHSteps(.5, .5, 0, 0);
    
    Leg backRight(&hservos[3], &hservos[2], 0, false);
    backRight.setKSteps(1, .5, .5, 1);
    backRight.setHSteps(.5, .5, 0, 0);
        
    for (int i = 0; i < NUM_SUB_STEPS*100; i++) {
        frontLeft.step();
        frontRight.step();
        backLeft.step();
        backRight.step();
        
        delay(stepDelay);
    }
    
    for (int i = 0; i < NUM_SERVOS; i++) {
        sservos[i].disable();
        hservos[i].disable();
    }
}

