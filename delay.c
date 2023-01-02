#include "delay.h"

void delay_us(uint32_t delay_us)
{
    uint32_t prev_timer_value, current_timer_value, time, end_time, prev_time;

#define TICK_OFFSET     (1)
    
    prev_timer_value = TMR1;
    // computes end time with pipeline delay compensation
    end_time = prev_timer_value + (delay_us * TIMER1_TICKS_PER_US);// - TICK_OFFSET;

    time = prev_timer_value;
    prev_time = prev_timer_value;

    while ( time < end_time )
    {
        current_timer_value = TMR1; // samples current time

        time +=  current_timer_value - prev_timer_value;
        if ( time < prev_time )
            time += TIMER1_PERIOD;  // in case timer roll off
        prev_time = time;
        prev_timer_value = current_timer_value;
    }
}

// measured durations with tick_offset = 17
// 1 => 1.28us
// 2 => 2.28us
// 3 => 3.28us
// 5 => 5.28us
// measured durations with tick_offset = 20
// 1 => 1.04us
// 2 => 2.04us
// 3 => 3.04us
// 5 => 5.02us
// 10 => 10us
// 20 => 20us

void delay_ms(uint16_t delay_ms)
{
    delay_us(1000*delay_ms);
}