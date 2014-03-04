#include <stdint.h>
#include "inc/hw_nvic.h"
#include "inc/hw_types.h"

#define unused __attribute__((unused))

void ResetISR(void);
void NmiSR(void);
void FaultISR(unsigned int * hardfault_args);
void IntDefaultHandler(void);

extern int main(void);
extern void PortDIntHandler(void);
extern void PortEIntHandler(void);
extern void ADC0SS0IntHandler(void);

// Symbols Created by the Linker
extern unsigned long _etext;
extern unsigned long _data;
extern unsigned long _edata;
extern unsigned long _bss;
extern unsigned long _ebss;
extern unsigned long _stack_bottom;
extern unsigned long _stack_top;
extern unsigned long _heap_bottom;
extern unsigned long _heap_top;

__attribute__ ((section(".isr_vector")))void (* const g_pfnVectors[])(void) =
{
    (void (*)(void))((unsigned long) &_stack_top),
    ResetISR,                               // The reset handler
    NmiSR,                                  // The NMI handler
    (void (*)(void)) FaultISR,              // The hard fault handler
    IntDefaultHandler,                      // The MPU fault handler
    IntDefaultHandler,                      // The bus fault handler
    IntDefaultHandler,                      // The usage fault handler
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    IntDefaultHandler,                      // SVCall handler
    IntDefaultHandler,                      // Debug monitor handler
    0,                                      // Reserved
    IntDefaultHandler,                      // The PendSV handler
    IntDefaultHandler,                      // The SysTick handler
    IntDefaultHandler,                      // GPIO Port A
    IntDefaultHandler,                      // GPIO Port B
    IntDefaultHandler,                      // GPIO Port C
    PortDIntHandler,                        // GPIO Port D
    PortEIntHandler,                        // GPIO Port E
    IntDefaultHandler,                      // UART0 Rx and Tx
    IntDefaultHandler,                      // UART1 Rx and Tx
    IntDefaultHandler,                      // SSI0 Rx and Tx
    IntDefaultHandler,                      // I2C0 Master and Slave
    IntDefaultHandler,                      // PWM Fault
    IntDefaultHandler,                      // PWM Generator 0
    IntDefaultHandler,                      // PWM Generator 1
    IntDefaultHandler,                      // PWM Generator 2
    IntDefaultHandler,                      // Quadrature Encoder 0
    ADC0SS0IntHandler,                      // ADC Sequence 0
    IntDefaultHandler,                      // ADC Sequence 1
    IntDefaultHandler,                      // ADC Sequence 2
    IntDefaultHandler,                      // ADC Sequence 3
    IntDefaultHandler,                      // Watchdog timer
    IntDefaultHandler,                      // Timer 0 subtimer A
    IntDefaultHandler,                      // Timer 0 subtimer B
    IntDefaultHandler,                      // Timer 1 subtimer A
    IntDefaultHandler,                      // Timer 1 subtimer B
    IntDefaultHandler,                      // Timer 2 subtimer A
    IntDefaultHandler,                      // Timer 2 subtimer B
    IntDefaultHandler,                      // Analog Comparator 0
    IntDefaultHandler,                      // Analog Comparator 1
    IntDefaultHandler,                      // Analog Comparator 2
    IntDefaultHandler,                      // System Control (PLL, OSC, BO)
    IntDefaultHandler,                      // FLASH Control
    IntDefaultHandler,                      // GPIO Port F
    IntDefaultHandler,                      // GPIO Port G
    IntDefaultHandler,                      // GPIO Port H
    IntDefaultHandler,                      // UART2 Rx and Tx
    IntDefaultHandler,                      // SSI1 Rx and Tx
    IntDefaultHandler,                      // Timer 3 subtimer A
    IntDefaultHandler,                      // Timer 3 subtimer B
    IntDefaultHandler,                      // I2C1 Master and Slave
    IntDefaultHandler,                      // Quadrature Encoder 1
    IntDefaultHandler,                      // CAN0
    IntDefaultHandler,                      // CAN1
    IntDefaultHandler,                      // CAN2
    IntDefaultHandler,                      // Ethernet
    IntDefaultHandler,                      // Hibernate
    IntDefaultHandler,                      // USB0
    IntDefaultHandler,                      // PWM Generator 3
    IntDefaultHandler,                      // uDMA Software Transfer
    IntDefaultHandler,                      // uDMA Error
    IntDefaultHandler,                      // ADC1 Sequence 0
    IntDefaultHandler,                      // ADC1 Sequence 1
    IntDefaultHandler,                      // ADC1 Sequence 2
    IntDefaultHandler,                      // ADC1 Sequence 3
    IntDefaultHandler,                      // I2S0
    IntDefaultHandler,                      // External Bus Interface 0
    IntDefaultHandler,                      // GPIO Port J
    IntDefaultHandler,                      // GPIO Port K
    IntDefaultHandler,                      // GPIO Port L
    IntDefaultHandler,                      // SSI2 Rx and Tx
    IntDefaultHandler,                      // SSI3 Rx and Tx
    IntDefaultHandler,                      // UART3 Rx and Tx
    IntDefaultHandler,                      // UART4 Rx and Tx
    IntDefaultHandler,                      // UART5 Rx and Tx
    IntDefaultHandler,                      // UART6 Rx and Tx
    IntDefaultHandler,                      // UART7 Rx and Tx
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    IntDefaultHandler,                      // I2C2 Master and Slave
    IntDefaultHandler,                      // I2C3 Master and Slave
    IntDefaultHandler,                      // Timer 4 subtimer A
    IntDefaultHandler,                      // Timer 4 subtimer B
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    IntDefaultHandler,                      // Timer 5 subtimer A
    IntDefaultHandler,                      // Timer 5 subtimer B
    IntDefaultHandler,                      // Wide Timer 0 subtimer A
    IntDefaultHandler,                      // Wide Timer 0 subtimer B
    IntDefaultHandler,                      // Wide Timer 1 subtimer A
    IntDefaultHandler,                      // Wide Timer 1 subtimer B
    IntDefaultHandler,                      // Wide Timer 2 subtimer A
    IntDefaultHandler,                      // Wide Timer 2 subtimer B
    IntDefaultHandler,                      // Wide Timer 3 subtimer A
    IntDefaultHandler,                      // Wide Timer 3 subtimer B
    IntDefaultHandler,                      // Wide Timer 4 subtimer A
    IntDefaultHandler,                      // Wide Timer 4 subtimer B
    IntDefaultHandler,                      // Wide Timer 5 subtimer A
    IntDefaultHandler,                      // Wide Timer 5 subtimer B
    IntDefaultHandler,                      // FPU
    IntDefaultHandler,                      // PECI 0
    IntDefaultHandler,                      // LPC 0
    IntDefaultHandler,                      // I2C4 Master and Slave
    IntDefaultHandler,                      // I2C5 Master and Slave
    IntDefaultHandler,                      // GPIO Port M
    IntDefaultHandler,                      // GPIO Port N
    IntDefaultHandler,                      // Quadrature Encoder 2
    IntDefaultHandler,                      // Fan 0
    0,                                      // Reserved
    IntDefaultHandler,                      // GPIO Port P (Summary or P0)
    IntDefaultHandler,                      // GPIO Port P1
    IntDefaultHandler,                      // GPIO Port P2
    IntDefaultHandler,                      // GPIO Port P3
    IntDefaultHandler,                      // GPIO Port P4
    IntDefaultHandler,                      // GPIO Port P5
    IntDefaultHandler,                      // GPIO Port P6
    IntDefaultHandler,                      // GPIO Port P7
    IntDefaultHandler,                      // GPIO Port Q (Summary or Q0)
    IntDefaultHandler,                      // GPIO Port Q1
    IntDefaultHandler,                      // GPIO Port Q2
    IntDefaultHandler,                      // GPIO Port Q3
    IntDefaultHandler,                      // GPIO Port Q4
    IntDefaultHandler,                      // GPIO Port Q5
    IntDefaultHandler,                      // GPIO Port Q6
    IntDefaultHandler,                      // GPIO Port Q7
    IntDefaultHandler,                      // GPIO Port R
    IntDefaultHandler,                      // GPIO Port S
    IntDefaultHandler,                      // PWM 1 Generator 0
    IntDefaultHandler,                      // PWM 1 Generator 1
    IntDefaultHandler,                      // PWM 1 Generator 2
    IntDefaultHandler,                      // PWM 1 Generator 3
    IntDefaultHandler                       // PWM 1 Fault
};


void ResetISR(void)
{
    unsigned long *pulSrc, *pulDest;

    //Copy the data segment initializers from flash to SRAM.
    pulSrc = &_etext;
    for(pulDest = &_data; pulDest < &_edata; ){
        *pulDest++ = *pulSrc++;
    }

    // Zero fill the bss segment.
    __asm("    ldr     r0, =_bss\n"
          "    ldr     r1, =_ebss\n"
          "    mov     r2, #0\n"
          "    .thumb_func\n"
          "zero_loop:\n"
          "        cmp     r0, r1\n"
          "        it      lt\n"
          "        strlt   r2, [r0], #4\n"
          "        blt     zero_loop");

    // FPU
    HWREG(NVIC_CPAC) = ((HWREG(NVIC_CPAC) & ~(NVIC_CPAC_CP10_M | NVIC_CPAC_CP11_M)) | NVIC_CPAC_CP10_FULL | NVIC_CPAC_CP11_FULL);

    // Main
    main();
}

void NmiSR(void)
{
    while(1);
}



__attribute__((naked))
void FaultISR( unsigned int * hardfault_args ){
  __asm(" .syntax unified\n"
    "movs r0, #4\n"
    "mov r1, lr\n"
    "tst r0, r1\n"
    "beq _MSP\n"
    "MRS r0, PSP\n"
    "B Hardfault_HandlerC\n"
  "_MSP:\n"
    "MRS r0, msp\n"
    "B Hardfault_HandlerC\n");
}

void Hardfault_HandlerC(unsigned long *hardfault_args){
    unused volatile unsigned long stacked_r0 = hardfault_args[0];
    unused volatile unsigned long stacked_r1 = hardfault_args[1];
    unused volatile unsigned long stacked_r2 = hardfault_args[2];
    unused volatile unsigned long stacked_r3 = hardfault_args[3];
    unused volatile unsigned long stacked_r12 = hardfault_args[4];
    unused volatile unsigned long stacked_lr = hardfault_args[5];
    unused volatile unsigned long stacked_pc = hardfault_args[6];
    unused volatile unsigned long stacked_psr = hardfault_args[7];
    unused volatile unsigned long _CFSR = *((volatile unsigned long*) 0xE000ED28);
    unused volatile unsigned long _HFSR = *((volatile unsigned long*) 0xE000ED2C);
    unused volatile unsigned long _DFSR = *((volatile unsigned long*) 0xE000ED30);
    unused volatile unsigned long _AFSR = *((volatile unsigned long*) 0xE000ED3C);
    unused volatile unsigned long _BFAR = *((volatile unsigned long*) 0xE000ED38);
    unused volatile unsigned long _MMAR = *((volatile unsigned long*) 0xE000ED34);
    __asm("BKPT #0\n");
}



void IntDefaultHandler(void)
{
    while(1);
}
