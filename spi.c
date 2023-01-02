#include "spi.h"
#include "system.h"

void init_spi1(uint32_t baudrate, uint8_t mode)
{
    // https://electrosome.com/spi/
    // Microchip has an inverted conception of the clock edge/phase

    SPI1CON = 0;
    SPI1STAT = 0;
    SPI1BUF = 0;
    SPI1BRG = PERIPHERAL_CLK/(2*baudrate) - 1;
    SPI1CONbits.MSTEN = 1;
    SPI1CONbits.MODE16 = 1;
    SPI1CONbits.MODE32 = 1;

    switch ( mode )
    {
        default:
        case SPI_MODE0:
            SPI1CONbits.CKE = 1;
            SPI1CONbits.CKP = 0;
            break;
        case SPI_MODE1:
            SPI1CONbits.CKE = 0;
            SPI1CONbits.CKP = 0;
            break;
        case SPI_MODE2:
            SPI1CONbits.CKE = 1;
            SPI1CONbits.CKP = 1;
            break;
        case SPI_MODE3:
            SPI1CONbits.CKE = 0;
            SPI1CONbits.CKP = 1;
            break;
    }

    SPI1CONbits.ON = 1;
}

uint8_t spi1_write_8bits(uint8_t data)
{
    while ( !SPI1STATbits.SPITBE ); // waits for available space in TX buffer
    SPI1CONbits.MODE16 = 0;
    SPI1CONbits.MODE32 = 0;
    SPI1BUF = data;                 // TX buffer write
    while ( !SPI1STATbits.SPIRBF ); // waits for RX buffer full
    return ( SPI1BUF );             // frees SPIRBF bit
}

uint32_t spi1_write_32bits(uint32_t data)
{
    while ( !SPI1STATbits.SPITBE ); // waits for available space in TX buffer
    SPI1CONbits.MODE16 = 1;
    SPI1CONbits.MODE32 = 1;
    SPI1BUF = data;                 // TX buffer write
    while ( !SPI1STATbits.SPIRBF ); // waits for RX buffer full
    return ( SPI1BUF );             // frees SPIRBF bit
}