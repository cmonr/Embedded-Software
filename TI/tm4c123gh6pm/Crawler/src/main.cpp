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

#define delay(x)      SysCtlDelay(SysCtlClockGet() / 3 * (x));


const int NUM_STEPS = 4;

class Leg {
protected:
SoftServo* knee;
SoftServo* hip;
int index;
int ksteps[NUM_STEPS];
int hsteps[NUM_STEPS];

public:
Leg(SoftServo* knee, SoftServo* hip, int phase = 0)
: knee(knee), hip(hip), index(phase) {}

void step(void) {
    knee->set(ksteps[index]);
    hip->set(hsteps[index]);
    index = (index+1)%NUM_STEPS;
}
};

class LeftLeg : public Leg {
public:
LeftLeg(SoftServo* knee, SoftServo* hip, int phase = 0)
: Leg(knee, hip, phase) {
    ksteps[0] = 0.0;
    hsteps[0] = 1.0;
    ksteps[1] = 0.5;
    hsteps[1] = 1.0;
    ksteps[2] = 0.5;
    hsteps[2] = 0.5;
    ksteps[3] = 0.0;
    hsteps[3] = 0.5;
}
};

class RightLeg : public Leg {
public:
RightLeg(SoftServo* knee, SoftServo* hip, int phase = 0)
: Leg(knee, hip, phase) {
    ksteps[0] = 0.0;
    hsteps[0] = 1.0;
    ksteps[1] = 0.5;
    hsteps[1] = 1.0;
    ksteps[2] = 0.5;
    hsteps[2] = 0.5;
    ksteps[3] = 0.0;
    hsteps[3] = 0.5;
}
};

class Crawler {
protected:
SoftServo servos[4];
int NUM_SERVOS;

public:
Crawler()
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

void crawlForward(void) {
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
};

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
