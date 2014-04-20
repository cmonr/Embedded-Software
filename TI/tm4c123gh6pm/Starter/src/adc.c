#include <stdbool.h>
#include <stdint.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/adc.h>
#include <driverlib/pin_map.h>
#include <driverlib/timer.h>
#include <driverlib/interrupt.h>

#include <inc/tm4c1233h6pm.h>

#include "adc.h"


uint32_t _adc[6];

void ADCInit(void)
{
    // ADC
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    
    //  GPIO (ADC Pins)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_2 | GPIO_PIN_3);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_TIMER, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH5);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_CH4);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 2, ADC_CTL_CH3);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 3, ADC_CTL_CH2);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 4, ADC_CTL_CH1);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 5, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
    ADCHardwareOversampleConfigure(ADC0_BASE, 64);
    
    ADCSequenceEnable(ADC0_BASE, 0);
    ADCIntEnable(ADC0_BASE, 0);

    //  Timer (ADC Trigger)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER5);
    TimerConfigure(TIMER5_BASE, TIMER_CFG_A_PERIODIC);
    TimerLoadSet(TIMER5_BASE, TIMER_A, SysCtlClockGet() / 10000); // 10KHz
    TimerControlTrigger(TIMER5_BASE, TIMER_A, true);
    TimerEnable(TIMER5_BASE, TIMER_A);

    IntEnable(INT_TIMER5A);
    IntEnable(INT_ADC0SS0); 
}


uint32_t ADCRead(unsigned char pin)
{
  return _adc[pin];
}


void ADC0SS0IntHandler(void)
{
    // Ack Interrupt
    ADCIntClear(ADC0_BASE, 0);

    // Read ADC Data
    ADCSequenceDataGet(ADC0_BASE, 0, _adc);
}
