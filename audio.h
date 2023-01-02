#ifndef _AUDIO_H
#define _AUDIO_H

#include "system.h"

void write_cs2000_8bits(uint8_t addr, uint8_t data);
void dump_cs2000_registers(void);

#endif