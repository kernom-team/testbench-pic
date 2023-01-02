#ifndef _IRQ_H
#define _IRQ_H

#include "system.h" 

extern uint32_t TRIGGER_10MS_TASKS, TRIGGER_100MS_TASKS;

void init_irq(void);

#endif