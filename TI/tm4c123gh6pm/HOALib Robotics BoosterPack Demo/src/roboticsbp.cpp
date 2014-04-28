#include "roboticsbp.h"


PCA9557 pca9557;
DRV8800 m0, m1, m2, m3;
Servo s0, s1, s2, s3, s4, s5;


void RoboticsBP_Init()
{
    //  IO Expander
    pca9557 = PCA9557(I2C0);
    pca9557.set(P3, LOW);   // Motors start disabled

    //  Motors
    m0 = DRV8800(PWM1_3, PF0);
    m1 = DRV8800(PWM1_2, PF4);
    m2 = DRV8800(PWM1_1, PC4);
    m3 = DRV8800(PWM1_0, PC7);
   
    //  Servos
    
    s0 = Servo(PWM0_0);
    s1 = Servo(PWM0_1);
    s2 = Servo(PWM0_2);
    s3 = Servo(PWM0_3);
    s4 = Servo(PWM0_4);
    s5 = Servo(PWM0_5);
}

void Motors_Enable()
{
    pca9557.set(P3, HIGH);
}

void Motors_Disable()
{
    pca9557.set(P3, LOW);
}
