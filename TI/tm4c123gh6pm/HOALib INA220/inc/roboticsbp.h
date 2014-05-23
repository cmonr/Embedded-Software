#ifndef _ROBOTICS_BP_
#define _ROBOTICS_BP_

#include "ina220.h"
#include "pca9557.h"
#include "drv8800.h"
#include "servo.h"


extern INA220 ina220;
extern PCA9557 pca9557;
extern DRV8800 m0, m1, m2, m3;
extern Servo s0, s1, s2, s3, s4, s5;


void RoboticsBP_Init( void );

void Motors_Enable( void );
void Motors_Disable( void );

#endif
