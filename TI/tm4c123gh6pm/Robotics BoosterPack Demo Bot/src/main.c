#define PART_TM4C123GH6PM 1

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <driverlib/gpio.h>
#include <driverlib/uart.h>
#include <driverlib/i2c.h>
#include <driverlib/pin_map.h>
#include <driverlib/interrupt.h>
#include <driverlib/sysctl.h>


#include "motor.h"
#include "i2c.h"
#include "encoder.h"
#include "servo.h"
//#include "adc.h"


#define toggleRed()    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, ~GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1))
#define toggleBlue()   GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, ~GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2))
#define toggleGreen()  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, ~GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_3))

#define delay(x)      SysCtlDelay(SysCtlClockGet() / 3 * x);


unsigned char i2c_buff[2];

void UART0IntHandler(){
    UARTIntClear(UART0_BASE, UARTIntStatus(UART0_BASE, true));
  
    UARTCharGet(UART0_BASE);

    toggleGreen();
}

void UART1IntHandler(){
    UARTIntClear(UART1_BASE, UARTIntStatus(UART1_BASE, true));

    

    toggleRed();
}


void configBluetooth()
{
    // Enter CMD mode
    printf("$$$");
    delay(1);

    // Set baud rate to 921600
    printf("U,921K,N\r");
    delay(1);
    
    // Exit CMD mode
    printf("---\r");

    // Reconfigure UART1
    UARTDisable(UART1_BASE);
    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 921600, (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE | UART_CONFIG_WLEN_8));
    UARTEnable(UART1_BASE);

    delay(5);
}

void init(void)
{
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


    // UART (Bluetooth)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
    GPIOPinConfigure(GPIO_PB0_U1RX);
    GPIOPinConfigure(GPIO_PB1_U1TX);
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE | UART_CONFIG_WLEN_8));
    
    UARTEnable(UART1_BASE);
    UARTIntEnable(UART1_BASE, UART_INT_RX);
    IntEnable(INT_UART1);
    setbuf(stdout, NULL);  // Disable printf buffer
 

    // Servos
    initServos();
    setServoLimits(0, 0.22, 0.59);
    setServoLimits(1, 0.35, 0.74);
    setServoLimits(2, 0.32, 0.57);
    setServoLimits(3, 0.32, 0.84);
    setServoLimits(5, 0.32, 0.82);

    invertServo(1);
    invertServo(5);

    setServo(0, 0);
    setServo(1, 0);
    setServo(2, 0);
    setServo(3, 0);
    setServo(5, 0);
    

    // Motors
    I2CInit();

    // PCA9557
    i2c_buff[0] = 0x03;
    i2c_buff[1] = 0x00; // 0: Output   1: Input
    I2CWrite(0x18, i2c_buff, 2);  // IO Direction

    i2c_buff[0] = 0x02;
    i2c_buff[1] = 0x00;
    I2CWrite(0x18, i2c_buff, 2);  // IO Polarity

    i2c_buff[0] = 0x01;
    i2c_buff[1] = 0x0F | 0x00; // Disable Motors
    I2CWrite(0x18, i2c_buff, 2);  // Output H/L

    initMotors();
    invertEncoder(0);


    // Enable Interrupts
    IntMasterEnable();
}


int currentServoNdx = 0;
int buffNdx = 0;
unsigned char buff[32];
char c;

void servoMenu()
{
    do
    {
        printf("\rS%d(%3d)>", currentServoNdx, (unsigned int) (getServoDuty(currentServoNdx) * 100));

        c = UARTCharGet(UART1_BASE);
        switch(c)
        {
            case 'w':
                setServo(currentServoNdx, getServoDuty(currentServoNdx) + 0.01);
                break;
            case 's':
                setServo(currentServoNdx, getServoDuty(currentServoNdx) - 0.01);
                break;
            case 'a':
                if (currentServoNdx != 0)
                    currentServoNdx--;
                break;
            case 'd':
                if (currentServoNdx != 5)
                    currentServoNdx++;
                break;
            default:
                printf("x%X\r\n", c);
        }


        UARTCharPut(UART0_BASE, c);
    }
    while(c != 0x1B); // Esc
}


void motorMenu()
{
    float speed = 0.8;

    i2c_buff[0] = 0x01;
    i2c_buff[1] = 0x0F | 0x80; // Enable Motors
    I2CWrite(0x18, i2c_buff, 2);  // Output H/L

    setMotor(0, 0.0);
    setMotor(1, 0.0);

    do
    {
        printf("\rM>");

        c = UARTCharGetNonBlocking(UART1_BASE);
        switch(c)
        {
            case 'w':
                setMotor(0,  speed);
                setMotor(1,  speed);
                break;
            case 's':
                setMotor(0, -speed);
                setMotor(1, -speed);
                break;
            case 'a':
                setMotor(0, -speed*1.2);
                setMotor(1,  speed*1.2);
                break;
            case 'd':
                setMotor(0,  speed*1.2);
                setMotor(1, -speed*1.2);
                break;
            default:
                if (c != 0xFF)
                    printf("x%X\r\n", c);

                setMotor(0, 0.0);
                setMotor(1, 0.0);
                break;
        }

        delay(0.05);
    }
    while(c != '\e'); // Esc
}



int main(void)
{ 
    unsigned char i2c_buff[2];
    
    init();

    //servoMenu();
    //motorMenu();

    while(1)
    {
        printf("%c[K\r>", 0x1B); // Clear line and prompt

        c = UARTCharGet(UART1_BASE);

        switch(c)
        {
            case 'm':
            case 'M': 
                motorMenu();
                break;
            case 's':
            case 'S':
                servoMenu();
                break;
        }
        
        toggleRed();
    }
   

    //Line Sensors
/*
    I2CInit();

    while(1)
    {
        int i;
        for(i=0; i<8; i++)
        {
            unsigned char err;
            
            
            i2c_buff[0] = 0x84 | (i << 4);
            I2CWrite(0x48, i2c_buff, 1);

            if ((err = I2CMasterErr(I2C0_BASE)) != 0)
            {
                printf("\e[K\r"); // Clear line
                printf("ERROR (TX): x%02x", err);
                delay(0.8);
                break;
            }


            I2CRead(0x48, i2c_buff, 1);

            if ((err = I2CMasterErr(I2C0_BASE)) != 0)
            {
                printf("\e[K\r"); // Clear line
                printf("ERROR (RX): x%02x", err);
                delay(0.8);
                break;
            }
            printf("%3d ", i2c_buff[0]);
        }
        
        delay(0.2);

        printf("\r");
    }*/
}
