#ifndef _SRC4392_H
#define _SRC4392_H

#define PORT_SLAVE  0
#define PORT_MASTER 1

extern const char* dir_freq[];
extern const uint8_t SRC4392_reg13[];

enum port_master_clock_source {
    PORT_MCLK = 0,
    PORT_RXCKI,
    PORT_RXCKO
};

#define DOUT_FROM_PORTA     0
#define DOUT_FROM_PORTB     1
#define DOUT_FROM_DIR       2
#define DOUT_FROM_ASRC      3

#define DIV_128     0
#define DIV_256     1
#define DIV_384     2
#define DIV_512     3

#define DIT_MCLK    0
#define DIT_RXCKO   1

void write_SRC4392(uint8_t addr, uint8_t data);
uint8_t read_SRC4392(uint8_t addr);
void init_SRC4392(uint8_t mode);

#endif
