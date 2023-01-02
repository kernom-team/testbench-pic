#include "delay.h"
#include "console.h"
#include "spi.h"
#include "src4392.h"
#include "system.h"

const char* dir_freq[] = {
"unknown",
"176.4k / 192k",
" 88.2k /  96k",
" 44.1k /  48k"
};

const uint8_t SRC4392_reg13[4] = {
0b00001001,
0b00001001,
0b00001011,
0b00001101
};

void write_SRC4392(uint8_t addr, uint8_t data)
{
    SRC4392_CSn_CLR;
    spi1_write_8bits(addr & 0x7F);  // byte 0
    spi1_write_8bits(0);            // byte 1
    spi1_write_8bits(data);         // byte 2
    SRC4392_CSn_SET;
}

uint8_t read_SRC4392(uint8_t addr)
{
    uint8_t data = 0;

    SRC4392_CSn_CLR;
    spi1_write_8bits(addr | 0x80);  // byte 0
    spi1_write_8bits(0);            // byte 1
    data = spi1_write_8bits(0);     // byte 2
    SRC4392_CSn_SET;

    return data;
}

void init_SRC4392(uint8_t mode)
{
    uint8_t reg03, reg03_ams, reg03_aouts;
    uint8_t reg04, reg04_adiv, reg04_aclk;
    uint8_t reg07, reg07_txdiv, reg07_txclk;
    
    if ( mode > 7 )
        mode = 0;

    switch ( mode )
    {
        default:
            reg03_ams = PORT_SLAVE;
            reg03_aouts = DOUT_FROM_ASRC;
            reg04_aclk = PORT_MCLK;
            reg04_adiv = DIV_128;
            reg07_txdiv = DIV_128;
            reg07_txclk = DIT_MCLK;
            break;

        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            reg03_ams = PORT_MASTER;
            reg03_aouts = DOUT_FROM_ASRC;
            reg04_aclk = PORT_MCLK;
            reg04_adiv = DIV_128;
            reg07_txdiv = DIV_128;
            reg07_txclk = DIT_MCLK;
            break;

        case 7:
            reg03_ams = PORT_MASTER;
            reg03_aouts = DOUT_FROM_DIR;
            reg04_aclk = PORT_RXCKO;
            reg04_adiv = DIV_128;
            reg07_txdiv = DIV_128;
            reg07_txclk = DIT_RXCKO;
            break;
    }

    SRC4392_RSTn_CLR;
    SRC4392_CSn_SET;
    delay_ms(10);
    SRC4392_RSTn_SET;
    delay_ms(10);

    write_SRC4392(0x01, 0b00111111);                                            // RST OFF | PDALL OFF | PORTA ON | PORTB ON | TX ON | RX ON | SRC ON

    // PORT A
    reg03 = 0b00000001 + (reg03_ams << 3) + (reg03_aouts << 4);
    write_SRC4392(0x03, reg03);
    reg04 = reg04_adiv + (reg04_aclk << 2);
    write_SRC4392(0x04, reg04);

    // PORT B : toujours slave
    write_SRC4392(0x05, 0b00000001);                                            // PORTB : SDOUTB = SDINB | slave | 24-bit I2S
    write_SRC4392(0x06, 0);

    // DIT
    reg07 = 0b00001000 + (reg07_txdiv << 5) + (reg07_txclk << 7);               // DIT : DATA source = PORTB
    write_SRC4392(0x07, reg07);
    write_SRC4392(0x08, 0b00000001);                                            // DIT : AESOUT = ON | TXOUT = OFF
    write_SRC4392(0x09, 0);

    // DIR
    write_SRC4392(0x0D, 0b00000000);                                            // DIR : RXCLK = RXCKI | RX INPUT = RX1
    write_SRC4392(0x0E, 0);                                                     // DIR : dynamically configured

    // DIR PLL
    // P = 2, J = 8, D = 0
    write_SRC4392(0x0F, 0b00100010);                                            // PLL1 config @ 24.5760MHz
    write_SRC4392(0x10, 0b00000000);                                            // PLL1 config @ 24.5760MHz
    write_SRC4392(0x11, 0b00000000);                                            // PLL1 config @ 24.5760MHz

    // GPIO
    write_SRC4392(0x1B, 0x0E);                                                  // GPIO1 : DIR Sync CLK
    write_SRC4392(0x1C, 0x0A);                                                  // GPIO2 : DIR Block start CLK
    write_SRC4392(0x1D, 0x0F);                                                  // GPIO3 : DIT Sync CLK
    write_SRC4392(0x1E, 0);                                                     // GPIO4 : GND

    // ASRC
    write_SRC4392(0x2D, 0b00000110);                                            // ASRC : MUTE OFF | CLKin = RXCKI | DATAin = DIRout
    write_SRC4392(0x2E, 0);                                                     // ASRC : factory config
    write_SRC4392(0x2F, 0);                                                     // ASRC out = 24bits
    write_SRC4392(0x30, 0);                                                     // ASRC LEFT  trim = 0dB
    write_SRC4392(0x31, 0);                                                     // ASRC RIGHT trim = 0dB
}
