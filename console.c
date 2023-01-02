#include <stdarg.h>
#include <stdio.h>

#include "console.h"
#include "uart.h"

void console_printf( const char *format, ... )
{
    va_list args;               // Declared in stdargs
    char buf[100];
//#warning " If buffer is to big (i.e. 1000), programm migh freeze"
    char *buf_ptr = buf; // Use pointer because char buff[100] is not a pointer for our compiler

    va_start( args, format );
    vsprintf( buf, format, args );        // Variable arguments version of sprintf
    va_end( args );
    
    while( *buf_ptr )
    {
        uart1_putc( *(buf_ptr++) );
    }
}