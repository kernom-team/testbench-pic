#ifndef _UART_H
#define _UART_H

#include "system.h"

void init_uart1(void);
void uart1_putc(char c);
char uart1_getc(void);
void uart1_basic_loopback(void);

#endif