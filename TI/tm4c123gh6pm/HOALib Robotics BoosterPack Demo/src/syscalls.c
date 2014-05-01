#include <stdint.h>
#include <sys/types.h>        //Needed for caddr_t

#include <inc/hw_memmap.h>    //Needed for GPIO Pins/UART base
#include <inc/hw_types.h>     //Needed for SysTick Types
#include <driverlib/rom.h>
#include <driverlib/rom_map.h>

#include "uart.h"

#define PRINTF_UART_TX UART1
#define PRINTF_UART_RX UART1

#define SILENT

char *heap_end = 0;

caddr_t _sbrk(unsigned int incr)
{
    extern unsigned long _heap_bottom;
    extern unsigned long _heap_top;
    static char *prev_heap_end;

    if (heap_end == 0) {
        heap_end = (caddr_t)&_heap_bottom;
    }

    prev_heap_end = heap_end;

    if (heap_end + incr > (caddr_t)&_heap_top) {
        return (caddr_t)0;
    }

    heap_end += incr;

    return (caddr_t) prev_heap_end;
}

int _close(int file)
{
    return -1;
}

int _fstat(int file)
{
    return -1;
}

int _isatty(int file)
{
    return -1;
}

int _lseek(int file, int ptr, int dir)
{
    return -1;
}

int _open(const char *name, int flags, int mode)
{
    return -1;
}

//TODO: getchar() does not function 
int _read(int file, char *ptr, int len)
{
    unsigned int i;
    for( i = 0; i < len; i++ ){
        ptr[i] = UART_ReadChar(PRINTF_UART_RX);
    }
    return len;
}

int _write(int file, char *ptr, unsigned int len)
{
    unsigned int i;
    for(i = 0; i < len; i++){
        #ifndef SILENT
        UART_WriteChar(PRINTF_UART_TX, ptr[i]);
        #endif
    }
    return i;
}
