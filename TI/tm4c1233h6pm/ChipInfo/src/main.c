#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>
#include <inc/hw_memmap.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/uart.h>
#include <driverlib/fpu.h>
#include <driverlib/rom.h>
#include <driverlib/rom_map.h>


void PrintMemoryLayout();
char *GetSP();

int main(void)
{
    // Clock
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);


    // UART
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 921600, (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE | UART_CONFIG_WLEN_8));
    
    UARTEnable(UART0_BASE);


    // FPU
    FPUEnable();
    FPULazyStackingEnable();


    // Memory Layout Info
    PrintMemoryLayout();


    // Sleep
    SysCtlSleep();


    return 0;
}


char* GetSP()
{
    char *stack_ptr;
    __asm (
        "mov %[stack_ptr], sp \r\n"
        : [stack_ptr] "=r" (stack_ptr)
        :
    );
    return stack_ptr;
}


void PrintMemoryLayout()
{
    extern unsigned long _text;
    extern unsigned long _etext;
    extern unsigned long _data;
    extern unsigned long _edata;
    extern unsigned long _bss;
    extern unsigned long _ebss;
    extern unsigned long _stack_bottom;
    extern unsigned long _stack_top;
    extern unsigned long _heap_bottom;
    extern char *heap_end;
    extern unsigned long _heap_top;

    
    printf("_text          = 0x%X\r\n",      (unsigned int) &_text);
    printf("_etext         = 0x%X\r\n",      (unsigned int) &_etext);
    printf("Sizeof(.text)  = %d bytes\r\n",  (&_etext - &_text) * 4);
    printf("---------------------------\r\n");
    printf("_data          = 0x%X\r\n",      (unsigned int) &_data);
    printf("_edata         = 0x%X\r\n",      (unsigned int) &_edata);
    printf("Sizeof(.data)  = %d bytes\r\n",  (&_edata - &_data) * 4);
    printf("---------------------------\r\n");
    printf("_bss           = 0x%X\r\n",      (unsigned int) &_bss);
    printf("_ebss          = 0x%X\r\n",      (unsigned int) &_ebss);
    printf("Sizeof(.bss)   = %d bytes\r\n",  (&_ebss - &_bss) * 4);
    printf("---------------------------\r\n");
    printf("_heap_bottom   = 0x%X\r\n",      (unsigned int) &_heap_bottom);
    printf("_heap_end      = 0x%X",        (unsigned int) heap_end);
    printf(" : usage: %d\r\n",               (heap_end - (char *) &_heap_bottom));
    printf("_heap_top      = 0x%X\r\n",      (unsigned int) &_heap_top);
    printf("Sizeof(heap)   = %d bytes\r\n",  (&_heap_top - &_heap_bottom) * 4);
    printf("---------------------------\r\n");
    printf("_stack_bottom  = 0x%X\r\n",      (unsigned int) &_stack_bottom);
    printf("StackPointer   = 0x%X",        (unsigned int) GetSP());
    printf(" : usage: %d\r\n",               ( (char *) &_stack_top - GetSP()));
    printf("_stack_top     = 0x%X\r\n",      (unsigned int) &_stack_top);
    printf("Sizeof(stack)  = %d bytes\r\n",  (&_stack_top - &_stack_bottom) * 4);
    printf("---------------------------\r\n");
}
