#include "button.h"
#include "irq.h"

uint32_t TRIGGER_10MS_TASKS, TRIGGER_100MS_TASKS;

void init_irq(void)
{
    // multi-vectored IRQ
    INTCONSET = _INTCON_MVEC_MASK;

    // T1 priority
    IPC1bits.T1IP = 1;
    IPC1bits.T1IS = 2;

    // U1 priority
    IPC8bits.U1IP = 1;
    IPC8bits.U1IS = 1;
}

void __ISR(_TIMER_1_VECTOR, ipl1AUTO) _T1Interrupt(void)
{
    IFS0CLR = 0x00000010;       // T1 IRQ flag cleared

    TPO_INV;
    
    static uint32_t tasks_10ms_cnt, tasks_100ms_cnt;

    if ( ++tasks_10ms_cnt >= 10 )
    {
        tasks_10ms_cnt = 0;
        TRIGGER_10MS_TASKS = 1;

        // button value sampling
        BT[SW0].electrical_state = SW0_PIN;
//        BT[SW1].electrical_state = SW1_PIN;
//        BT[DUT1].electrical_state = DUT1_DETECT_PIN;
//        BT[DUT2].electrical_state = DUT2_DETECT_PIN;
//        BT[DUT3].electrical_state = DUT3_DETECT_PIN;

        // press duration counter
        count_button_state(SW0);
        count_button_state(SW1);
//        count_button_state(DUT1);
//        count_button_state(DUT2);
//        count_button_state(DUT3);
    }

    if ( ++tasks_100ms_cnt >= 100 )
    {
        tasks_100ms_cnt = 0;
        TRIGGER_100MS_TASKS = 1;
    }
}
