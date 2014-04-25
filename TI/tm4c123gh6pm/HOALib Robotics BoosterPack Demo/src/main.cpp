#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "pin.h"
#include "pwm.h"
#include "uart.h"

#include "roboticsbp.h"
#include "tests.h"

#define rLED  PWM1_5 
#define gLED  PWM1_7
#define bLED  PWM1_6


#define CMD_DELIM '\n'

unsigned char* buff;
unsigned char buffptr = 0;


unsigned char cmdBuff[32];


void parseCmd()
{   
    static float value;
    /* Valid commands:
    
    //  LED Comamnds
    #           [r,g,b,*]
    L#E         Enable LED #
    L#D         Disable LED #
    L#T=0.3     Throttle LED # to 30%
    L#=0.6      Set LED to 60%

    //  Motor Commands
    #           [0-3, *]
    M#E         Enable Motor #
    M#D         Disable Motor #
    M#I         Invert Motor #
    M#T=0.7     Throttle Motor # to 70%
    M#=0.3      Set Motor # to 30% Fwd
    M#=-0.8     Set Motor # to 80% Rev
   
    //  Servo Commands
    #           [0-5, *]
    S#E         Enable Servo #
    S#D         Disable Servo #
    S#I         Invert Servo #
    S#L=0.4,0.6 Set Servo 3 limits to 40% - 60%
    S#=0.2      Set Servo 2 to 20%

    TODO: Add mmore targets later
    */
   
    // Determine object to do command on
    switch(cmdBuff[0])
    {
        case 'm':                   // Motor command
        case 'M':
            switch(cmdBuff[1])
            {
                case '*':           // Command all motors
                    switch(cmdBuff[2])
                    {
                        case 'e':   // Enable Motor(s)
                        case 'E':
                            Motors_Enable();
                            printf(" All motors enabled\r\n");
                            break;

                        case 'd':   // Disable Motor(s)
                        case 'D':
                            Motors_Disable();
                            printf(" All motors disabled\r\n");
                            break;

                        case 'i':   // Invert Motor(s)
                        case 'I':
                            m0.invert();
                            m1.invert(); 
                            m2.invert(); 
                            m3.invert(); 
                            printf(" All motors inverted\r\n All motors set to  0%%\r\n");
                            break;

                        case 't':   // Throttle Motor(s)
                        case 'T':
                            // Parse throttle value
                            value = strtof(reinterpret_cast<const char*>(&cmdBuff[4]), NULL); 
                            /*
                            if (sscanf(&cmdBuff[3], "=%f", &value) != 1)
                            {
                                printf("Invalid motor trottle value: %s\r\n", cmdBuff);
                                break;
                            }*/

                            // Check throttle value range
                            if (value > 1 || value < 0)
                            {
                                printf("Invalid motor throttle value: %s\r\n", cmdBuff);
                                break;
                            }
                            
                            // Throttle motors
                            m0.throttle(value);
                            m1.throttle(value);
                            m2.throttle(value);
                            m3.throttle(value);

                            printf(" All motors throttled to % 2d%%\r\n All motors set to 0%%", (unsigned char) (value*100));
                            break;

                        case '=':   //Set Motor
                            // Parse throttle value
                            value = strtof(reinterpret_cast<const char*>(&cmdBuff[3]), NULL); 
                            /*
                            if (sscanf(&cmdBuff[2], "=%f", &value) != 1)
                            {
                                printf("Invalid motor value: %s\r\n", cmdBuff);
                                break;
                            }*/

                            // Check throttle value range
                            if (value > 1 || value < -1)
                            {
                                printf("Invalid motor value: %s\r\n", cmdBuff);
                                break;
                            }
                            
                            // Throttle motors
                            m0.set(value);
                            m1.set(value);
                            m2.set(value);
                            m3.set(value);

                            printf(" All motors set to % 2d%%\r\n", (unsigned char) (value*100));
                            break;

                        default:
                            printf("Invalid motor command: %s\r\n", cmdBuff);
                    }
                    break; 

                case '0':
                case '1':
                case '2':
                case '3':
                    switch(cmdBuff[2])
                    {
                        case 'e':   // Enable Motor(s)
                        case 'E':
                            
                            break;
                        case 'd':   // Disable Motor(s)
                        case 'D':
                            break;
                        case 'i':   // Invert Motor(s)
                        case 'I':
                            break;
                        case 't':   // Throttle Motor(s)
                        case 'T':
                            break;
                        case '=':   //Set Motor
                            break;
                        default:
                            printf("Invalid Motor command: %s\r\n", cmdBuff);
                    }
                    break;

                default:
                    printf("Invalid Motor index: %s\r\n", cmdBuff);
            }
            break;

        case 's':   // Servo command
        case 'S':
            break;

        default:
            printf("Invalid Motor target: %s\r\n", cmdBuff);
    }   

    // Reset CMD Buff
    memset(cmdBuff, 0, 32);
}

void UART0_RX_IRQ()
{
    // Read cahracter
    unsigned char data = UART_ReadChar(UART0);

    buff[buffptr++] = data;

    // TODO: Add check for max buffer size

    if (data == CMD_DELIM)
    {
       // Copy string into seperate buffer
       memcpy(cmdBuff, buff, 32);

       // Zero off buffer
       memset(buff, 0, 32);
       buffptr = 0;

       // Parse command
       parseCmd();
    }
}


int main(void)
{
    // Clock (80MHz)
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);


    // Init peripherals
    //  UART0
    UART_Init(UART0);
    UART_SetIRQ(UART0, UART_RX_IRQ, &UART0_RX_IRQ);
    UART_IntEnable(UART0, UART_RX_IRQ);
    UART_Enable(UART0);
    setbuf(stdout, NULL);

    //  I2C0
    I2C_Init(I2C0);
    I2C_Enable(I2C0);

    //  PWM1
    //PWM_Init(PWM0, 50);
    PWM_Init(PWM1, 1000);
    PWM_Enable(rLED);
    PWM_Enable(gLED);
    PWM_Enable(bLED);
    
    // NVIC
    IntMasterEnable();


    // Init Robotics BoosterPack
    RoboticsBP_Init();

    
    // Power On Self Test
    printf("POST Started");

    //  LEDs
    ledTest(rLED);    
    ledTest(gLED);    
    ledTest(bLED);    

    //  Motors
    //   First enable motors
    pca9557.set(P3, HIGH);
    motorTest(&m0);
    motorTest(&m1);
    motorTest(&m2);
    motorTest(&m3);
    pca9557.set(P3, LOW);

    //  Servos
    servoTest(&s0);
    servoTest(&s1);
    servoTest(&s2);
    servoTest(&s3);
    servoTest(&s4);
    servoTest(&s5);

    printf("\rPOST Completed\r\n\n");

    while(1);
}

