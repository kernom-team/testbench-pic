#include "uart.h"
#include "delay.h"

uint8_t g_flag_dut1_sequence_finished = 0;
uint8_t g_flag_dut2_sequence_finished = 0;
uint8_t g_flag_dut3_sequence_finished = 0;

void init_uart1(void)
{
    IFS1bits.U1RXIF = 0;
    IEC1bits.U1RXIE = 0;

    U1MODE = 0;
    U1STA = 0;
    U1BRG = ((PERIPHERAL_CLK/(4*UART1_BAUDRATE))-1);

    U1MODEbits.BRGH = 1;
    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;

    IEC1bits.U1RXIE = 0;
    delay_ms(1);
    U1MODEbits.UARTEN = 1;
}

void uart1_putc(char c)
{
    while ( U1STAbits.UTXBF == 1 );     // waits until end of previous transfer
    U1TXREG = c;
}

void __ISR(_UART_1_VECTOR, ipl1AUTO) _UART1Interrupt(void)
{
    IFS1bits.U1RXIF = 0;

    LED0_INV;
    
    char c;
    if ( U1STAbits.URXDA )
    {
        c = U1RXREG;
                
        U1TXREG = c;
    }
}

char uart1_getc(void)
{
    char c = '\0';

    if ( U1STAbits.URXDA )
    {
       c = U1RXREG;
    }
    
    return c;
}

void uart1_basic_loopback(void)
{
    if ( U1STAbits.URXDA )
    {
       char c;
       c = U1RXREG;
       uart1_putc(c);
    }
}
