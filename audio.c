#include "audio.h"
#include "console.h"
#include "i2c.h"

#define CS2000_I2C_ADDR     (0b1001110)

#define CS2000_REGMAP_SIZE  (0x1E)

uint8_t cs2000_wr_data[CS2000_REGMAP_SIZE+1];
uint8_t cs2000_rd_data[CS2000_REGMAP_SIZE+1];

void configure_audio_mode(uint8_t mode)
{
    if ( mode > 7 )
        mode = 0;
}

void cs2000_write_ratio(uint32_t ratio, uint32_t num)
{
    uint8_t base_addr;

    switch ( num )
    {
        default:
            base_addr = 0x06;
            break;
        case 1:
            base_addr = 0x0A;
            break;
        case 2:
            base_addr = 0x0E;
            break;
        case 3:
            base_addr = 0x12;
            break;
    }

    cs2000_wr_data[0] = 0x80 | base_addr;  // MAP bit = 1 + base address
    cs2000_wr_data[1] = (ratio >> 24) & 0xFF;
    cs2000_wr_data[2] = (ratio >> 16) & 0xFF;
    cs2000_wr_data[3] = (ratio >>  8) & 0xFF;
    cs2000_wr_data[4] = ratio & 0xFF;
    I2C2_Write(CS2000_I2C_ADDR, cs2000_wr_data, 5);
    delay_ms(2);
}

void write_cs2000_8bits(uint8_t addr, uint8_t data)
{
    cs2000_wr_data[0] = addr;  // MAP bit = 0 | base address = addr
    cs2000_wr_data[1] = data;
    I2C2_Write(CS2000_I2C_ADDR, cs2000_wr_data, 2);
    delay_ms(1);
}

void init_cs2000(uint8_t mode)
{
    uint8_t ver, i;
    uint32_t ratio;

    // turn control port mode ON
    // EnDevCfg1 and EnDevCfg2 must be set to 1
    write_cs2000_8bits(0x03, 0x05);
    write_cs2000_8bits(0x05, 0x01);

    switch( mode )
    {
        default:
            ratio = 0x00080000;//; // x128
            break;

        case 1:
            ratio = 0x3ACCD;
            break;

        case 2:
            ratio = 0x40000;
            break;

        case 3:
            ratio = 0x7599A;
            break;

        case 4:
            ratio = 0x80000;
            break;

        case 5:
            ratio = 0xEB333;
            break;

        case 6:
            ratio = 0x100000;
            break;

        case 7:
            ratio = 0;
            break;
    }

    if ( mode == 0 )
        write_cs2000_8bits(0x04, 0x01);
    else
        write_cs2000_8bits(0x04, 0x00);

    cs2000_write_ratio(ratio, 0);


    // applies proper reference clock input divider
    write_cs2000_8bits(0x16, 0x08);

    write_cs2000_8bits(0x17, 0x10);

    dump_cs2000_registers();
}

void dump_cs2000_registers(void)
{
    uint8_t i;

    cs2000_wr_data[0] = 0x81;  // MAP bit = 1 | base address = 0x01
    I2C2_WriteRead(CS2000_I2C_ADDR, cs2000_wr_data, 1, cs2000_rd_data, CS2000_REGMAP_SIZE);

    delay_ms(5);

    for (i=0;i<CS2000_REGMAP_SIZE;i++)
        console_printf("CS2000 %02x-%02x\r\n", i+1, cs2000_rd_data[i]);
}