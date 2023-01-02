#include "button.h"

button_t BT[AVAILABLE_BUTTONS];

void init_button(void)
{
    clear_button_state(SW0);
    clear_button_state(SW1);
//    clear_button_state(DUT1);
//    clear_button_state(DUT2);
//    clear_button_state(DUT3);
}

void clear_button_state(uint8_t button)
{
    if ( button < AVAILABLE_BUTTONS )
    {
        BT[button].electrical_state = BUTTON_IDLE_VALUE;
        BT[button].pressed = NOT_PRESSED;
        BT[button].status = NOT_PRESSED;
        BT[button].status_ack = 1;
    }
}

void count_button_state(uint8_t button)
{
    if ( button < AVAILABLE_BUTTONS )
    {
        if ( BT[button].electrical_state == BUTTON_IDLE_VALUE  )
        {
            if ( (BT[button].press_cnt >= SHORT_PRESS_DURATION) &&
                 (BT[button].press_cnt < LONG_PRESS_DURATION) )
            {
                BT[button].pressed = SHORT_PRESSED;
                BT[button].release_cnt = BUTTON_TIMEOUT_DURATION;
            }
            if ( (BT[button].press_cnt >= LONG_PRESS_DURATION) &&
                 (BT[button].press_cnt < VERY_LONG_PRESS_DURATION) )
            {
                BT[button].pressed = LONG_PRESSED;
                BT[button].release_cnt = BUTTON_TIMEOUT_DURATION;
            }
            BT[button].press_cnt = 0;
            BT[button].release_cnt--;
            if ( !BT[button].release_cnt || BT[button].status_ack )
            {
                BT[button].pressed = NOT_PRESSED;
                BT[button].status_ack = 0;
            }
        }
        else
        {
            if ( BT[button].press_cnt < 0xFFFF )
                BT[button].press_cnt++;
            if ( !BT[button].status_ack )
            {
                if ( BT[button].press_cnt >= LONG_PRESS_DURATION )
                {
                    BT[button].pressed = LONG_PRESSED;
                    BT[button].release_cnt = BUTTON_TIMEOUT_DURATION;
                }
            }
            if ( (BT[button].pressed == WAITING_FOR_RELEASE_AFTER_LONG_PRESS) &&
                 (BT[button].press_cnt >= VERY_LONG_PRESS_DURATION) )
            {
                BT[button].pressed = VERY_LONG_PRESSED;
                BT[button].release_cnt = BUTTON_TIMEOUT_DURATION;
            }
        }
    }
}

void get_button_state(uint8_t button)
{
    uint8_t state = NOT_PRESSED;

    if ( button < AVAILABLE_BUTTONS )
    {
        state = BT[button].pressed;
        switch( state )
        {
            default:
                break;
            case SHORT_PRESSED:
                BT[button].status_ack = 1;
                BT[button].pressed = WAITING_FOR_RELEASE_AFTER_SHORT_PRESS;
                break;
            case LONG_PRESSED:
                BT[button].status_ack = 1;
                BT[button].pressed = WAITING_FOR_RELEASE_AFTER_LONG_PRESS;
                break;
            case VERY_LONG_PRESSED:
                BT[button].status_ack = 1;
                BT[button].pressed = WAITING_FOR_RELEASE_AFTER_VERY_LONG_PRESS;
                break;
        }
        BT[button].status = state;
    }
}

void get_all_buttons_state(void)
{
    get_button_state(SW0);
    get_button_state(SW1);
//    get_button_state(DUT1);
//    get_button_state(DUT2);
//    get_button_state(DUT3);
}