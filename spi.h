#ifndef _SPI_H
#define _SPI_H

#include "system.h"

enum spi_mode {
    SPI_MODE0,
    SPI_MODE1,
    SPI_MODE2,
    SPI_MODE3
};

void init_spi1(uint32_t baudrate, uint8_t mode);
uint8_t spi1_write_8bits(uint8_t data);
uint32_t spi1_write_32bits(uint32_t data);

#endif