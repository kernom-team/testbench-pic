#ifndef _BUTTON_H
#define _BUTTON_H

#include "system.h"

typedef struct{
    uint8_t mode;
    uint8_t electrical_state;
    uint8_t pressed;
    uint8_t status;
    uint8_t status_ack;
    uint16_t press_cnt;
    uint16_t release_cnt;
}button_t;

extern button_t BT[];

enum button_list {
    SW0,
    SW1,
//    DUT1,
//    DUT2,
//    DUT3,
    AVAILABLE_BUTTONS
};

// electrical value
#define BUTTON_IDLE_VALUE           (1)
#define BUTTON_ACTIVE_VALUE         (0)

// duration in IRQ processing count
#define SHORT_PRESS_DURATION        (5)
#define LONG_PRESS_DURATION         (60)
#define VERY_LONG_PRESS_DURATION    (400)
#define BUTTON_TIMEOUT_DURATION     (100)

// button state machine values
enum button_state {
    NOT_PRESSED,
    WAITING_FOR_RELEASE_AFTER_SHORT_PRESS,
    WAITING_FOR_RELEASE_AFTER_LONG_PRESS,
    WAITING_FOR_RELEASE_AFTER_VERY_LONG_PRESS,
    SHORT_PRESSED,
    LONG_PRESSED,
    VERY_LONG_PRESSED
};

void init_button(void);
void clear_button_state(uint8_t button);
void count_button_state(uint8_t button);
void get_button_state(uint8_t button);
void get_all_buttons_state(void);

#endif