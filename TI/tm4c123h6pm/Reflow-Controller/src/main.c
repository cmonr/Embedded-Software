#define PART_TM4C123GH6PM 1

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "driverlib/ssi.h"
#include "driverlib/fpu.h"
#include "driverlib/timer.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"


#define ON  0xFF
#define OFF 0x00

#define PIDHz 100


void empty(void){}


void (*Timer0AHandler)(void) = &empty;
void (*SysTickHandler)(void) = &empty;


void Timer0AIntHandler(void)
{
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    Timer0AHandler();
}


void SysTickIntHandler(void){
    SysTickHandler();
}




static unsigned int thermocouple;

static float internal_temp;
static float thermocouple_temp;

void readThermocouple()
{
    unsigned long tmp;
        
    // Toggle CS
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, OFF);
    
    // "Clock" bus to get data in
    SSIDataPut(SSI0_BASE, 0);
    SSIDataGet(SSI0_BASE, &tmp);
    thermocouple = (tmp & 0xFF) << 24;
    
    SSIDataPut(SSI0_BASE, 0);
    SSIDataGet(SSI0_BASE, &tmp);
    thermocouple |= ((tmp & 0xFF) << 16);
     
    SSIDataPut(SSI0_BASE, 0);
    SSIDataGet(SSI0_BASE, &tmp);
    thermocouple |= ((tmp & 0xFF) << 8);
    
    SSIDataPut(SSI0_BASE, 0);
    SSIDataGet(SSI0_BASE, &tmp);
    thermocouple |= (tmp & 0xFF);

    // Toggle CS
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, ON);

   
    // Convert to proper precision
    internal_temp = ((float) ((thermocouple >> 4) & 0x7FF)) *.0625;
    thermocouple_temp = ((float) ((thermocouple >> 18) & 0x03FF)) * 0.25;
}



static float in, out, setPt = 0;
static float I, lastIn;
static float kp = -500,   // This should probably be tuned...
             ki = 0 * PIDHz,
             kd = 0 / PIDHz;
#define outMax 2047
#define outMin 2 

void ControlOven(void)
{
    float err, dIn;
  
    // Get New Data
    readThermocouple();
    in = thermocouple_temp;

    // Do a little math
    err = setPt - in;
    I += (ki*err);
    if (I > outMax)
        I = outMax;
    else if (I < outMin)
        I = outMin;
    dIn = in - lastIn;

    // Set output
    out = kp*err + I - kd*dIn;
    if (out > outMax)
        out = outMax;
    else if (out < outMin)
        out = outMin;
    TimerMatchSet(TIMER3_BASE, TIMER_A, out - 1);

    // Save these for next time
    lastIn = in;
}



static char debugCtr = 0;

  
void DebugOut(void)
{
    // Print Data
    readThermocouple();
    printf("%f, %.2f\r\n", out, thermocouple_temp);

    // Heartbeat
    if (debugCtr == 4)
    {
      GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, ~(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1)));
      debugCtr -= 5;
    }

    debugCtr++;
}



int main(void)
{
    // Clock (80 MHz)
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    
    
    // GPIO
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, OFF);

    
    // UART
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 921600, (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE | UART_CONFIG_WLEN_8));

    UARTEnable(UART0_BASE);

    
    // SPI
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
    GPIOPinConfigure(GPIO_PA2_SSI0CLK);
    GPIOPinConfigure(GPIO_PA4_SSI0RX);
    GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_4);
    SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 10000000, 8);

    // Manually Toggle CS (32 bit data)
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_3);
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, ON);

    // MAX31855 Power
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_7);
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7, ON);

    // MAX31855 GND
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_6);
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, OFF);

    SSIEnable(SSI0_BASE);


    // FPU
    FPULazyStackingEnable();
    FPUEnable();


    // Timer
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet() / PIDHz);
    
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER0_BASE, TIMER_A);

    Timer0AHandler = &ControlOven;

    
    // Timer/PWM
    GPIOPinConfigure(GPIO_PB2_T3CCP0);
    GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_2);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
    TimerConfigure(TIMER3_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM);
    TimerLoadSet(TIMER3_BASE, TIMER_A, outMax);
    TimerMatchSet(TIMER3_BASE, TIMER_A, outMax - 1);

    TimerEnable(TIMER3_BASE, TIMER_A);


    // Systick
    SysTickPeriodSet(SysCtlClockGet() / 10);

    SysTickIntEnable();
    SysTickEnable();

    SysTickHandler = &DebugOut;
   
    // Enable Interrupts
    IntMasterEnable();
      
    while(1)
    {
        float temps[] = {110, 160, 200, 140};
        unsigned short delay[] = {90, 120, 20, 300};
        short i;

        for(i=0; i<4; i++)
        {
            printf("\r\n");
            setPt = temps[i];
            SysCtlDelay(SysCtlClockGet() * delay[i] / 3);
        }

        // Add code to wait for button press
    } 
}
