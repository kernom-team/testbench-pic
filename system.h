#ifndef _SYSTEM_H
#define _SYSTEM_H

#include <xc.h>
#include <sys/attribs.h>

//=== MACROS ===
#define nop     asm("nop")
#define di      asm("DI")
#define ei      asm("EI")
#define clrwdt  (WDTCONSET = 0x00000001)


#define CPU_CLK                     (40000000)      // [Hz]
#define PERIPHERAL_CLK              (CPU_CLK)       // [Hz]
#define TIMER1_CLK                  (CPU_CLK/8)     // [Hz]

#define TIMER1_IRQ_FREQ             (1000)          // [Hz]
#define TIMER1_PERIOD               (TIMER1_CLK/TIMER1_IRQ_FREQ)
#define TIMER1_TICKS_PER_US         (TIMER1_CLK/1000000)

#define UART1_BAUDRATE              (115200)

//=== ADC INDEX ===
#define TESTBENCH_REV_ADC_CHANNEL   (0)

#define ADC_SCALING                 (5)

//=== LEDs ===
#define LED0_IN                 (TRISASET = _TRISA_TRISA1_MASK)
#define LED0_OUT                (TRISACLR = _TRISA_TRISA1_MASK)
#define LED0_SET                (LATASET = _LATA_LATA1_MASK)
#define LED0_CLR                (LATACLR = _LATA_LATA1_MASK)
#define LED0_INV                (LATAINV = _LATA_LATA1_MASK)

#define LED1_IN                 (TRISBSET = _TRISB_TRISB2_MASK)
#define LED1_OUT                (TRISBCLR = _TRISB_TRISB2_MASK)
#define LED1_SET                (LATBSET = _LATB_LATB2_MASK)
#define LED1_CLR                (LATBCLR = _LATB_LATB2_MASK)
#define LED1_INV                (LATBINV = _LATB_LATB2_MASK)

#define LED2_IN                 (TRISBSET = _TRISB_TRISB3_MASK)
#define LED2_OUT                (TRISBCLR = _TRISB_TRISB3_MASK)
#define LED2_SET                (LATBSET = _LATB_LATB3_MASK)
#define LED2_CLR                (LATBCLR = _LATB_LATB3_MASK)
#define LED2_INV                (LATBINV = _LATB_LATB3_MASK)

//=== DUT ===
#define LED_DUT1_IN             (TRISBSET = _TRISB_TRISB13_MASK)
#define LED_DUT1_OUT            (TRISBCLR = _TRISB_TRISB13_MASK)
#define LED_DUT1_SET            (LATBSET = _LATB_LATB13_MASK)
#define LED_DUT1_CLR            (LATBCLR = _LATB_LATB13_MASK)
#define LED_DUT1_INV            (LATBINV = _LATB_LATB13_MASK)

#define LED_DUT2_IN             (TRISBSET = _TRISB_TRISB14_MASK)
#define LED_DUT2_OUT            (TRISBCLR = _TRISB_TRISB14_MASK)
#define LED_DUT2_SET            (LATBSET = _LATB_LATB14_MASK)
#define LED_DUT2_CLR            (LATBCLR = _LATB_LATB14_MASK)
#define LED_DUT2_INV            (LATBINV = _LATB_LATB14_MASK)

#define LED_DUT3_IN             (TRISBSET = _TRISB_TRISB15_MASK)
#define LED_DUT3_OUT            (TRISBCLR = _TRISB_TRISB15_MASK)
#define LED_DUT3_SET            (LATBSET = _LATB_LATB15_MASK)
#define LED_DUT3_CLR            (LATBCLR = _LATB_LATB15_MASK)
#define LED_DUT3_INV            (LATBINV = _LATB_LATB15_MASK)

#define DUT1_DETECT_IN          (TRISBSET = _TRISB_TRISB10_MASK)
#define DUT1_DETECT_PIN         (_RB10)

#define DUT2_DETECT_IN          (TRISBSET = _TRISB_TRISB11_MASK)
#define DUT2_DETECT_PIN         (_RB11)

#define DUT3_DETECT_IN          (TRISBSET = _TRISB_TRISB12_MASK)
#define DUT3_DETECT_PIN         (_RB12)

#define CMD_DUT1_9V_OUT         (TRISACLR = _TRISA_TRISA2_MASK)
#define CMD_DUT1_9V_SET         (LATASET = _LATA_LATA2_MASK)
#define CMD_DUT1_9V_CLR         (LATACLR = _LATA_LATA2_MASK)
 
#define CMD_DUT2_9V_OUT         (TRISACLR = _TRISA_TRISA3_MASK)
#define CMD_DUT2_9V_SET         (LATASET = _LATA_LATA3_MASK)
#define CMD_DUT2_9V_CLR         (LATACLR = _LATA_LATA3_MASK)

#define CMD_DUT3_9V_OUT         (TRISBCLR = _TRISB_TRISB4_MASK)
#define CMD_DUT3_9V_SET         (LATBSET = _LATB_LATB4_MASK)
#define CMD_DUT3_9V_CLR         (LATBCLR = _LATB_LATB4_MASK)

//=== TPs ===
#define TPO_OUT                 (TRISBCLR = _TRISB_TRISB5_MASK)
#define TPO_IN                  (TRISBSET = _TRISB_TRISB5_MASK)
#define TPO_SET                 (LATBSET = _LATB_LATB5_MASK)
#define TPO_CLR                 (LATBCLR = _LATB_LATB5_MASK)
#define TPO_INV                 (LATBINV = _LATB_LATB5_MASK)

#define TPV_OUT                 (TRISACLR = _TRISA_TRISA4_MASK)
#define TPV_IN                  (TRISASET = _TRISA_TRISA4_MASK)
#define TPV_SET                 (LATASET = _LATA_LATA4_MASK)
#define TPV_CLR                 (LATACLR = _LATA_LATA4_MASK)
#define TPV_INV                 (LATAINV = _LATA_LATA4_MASK)

//=== PUSH BUTTON ===
#define SW0_IN              (TRISBSET = _TRISB_TRISB9_MASK)
#define SW0_PIN             (_RB9)

#define SW1_IN              (TRISBSET = _TRISB_TRISB8_MASK)
#define SW1_PIN             (_RB8)

extern uint8_t g_flag_dut1_sequence_finished;
extern uint8_t g_flag_dut2_sequence_finished;
extern uint8_t g_flag_dut3_sequence_finished;

#endif