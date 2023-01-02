#include "button.h"

#include "delay.h"
#include "irq.h"
#include "uart.h"
#include "system.h"
#include "console.h"

// DEVCFG3
#pragma config USERID = 0xFFFF          // Enter Hexadecimal value (Enter Hexadecimal value)
#pragma config PMDL1WAY = OFF           // Peripheral Module Disable Configuration (Allow multiple reconfigurations)
#pragma config IOL1WAY = OFF            // Peripheral Pin Select Configuration (Allow multiple reconfigurations)

// DEVCFG2
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
#pragma config FPLLMUL = MUL_20         // PLL Multiplier (20x Multiplier)
#pragma config FPLLODIV = DIV_2         // System PLL Output Clock Divider (PLL Divide by 2)

// DEVCFG1
#pragma config FNOSC = FRCPLL           // Oscillator Selection Bits (Fast RC Osc with PLL)
#pragma config FSOSCEN = OFF            // Secondary Oscillator Enable (Disabled)
#pragma config IESO = OFF               // Internal/External Switch Over (Disabled)
#pragma config POSCMOD = OFF            // Primary Oscillator Configuration (Primary osc disabled)
#pragma config OSCIOFNC = OFF           // CLKO Output Signal Active on the OSCO Pin (Disabled)
#pragma config FPBDIV = DIV_1           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/1)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
#pragma config WDTPS = PS1048576        // Watchdog Timer Postscaler (1:1048576)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable (Watchdog Timer is in Non-Window Mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))
#pragma config FWDTWINSZ = WINSZ_50     // Watchdog Timer Window Size (Window Size is 50%)

// DEVCFG0
#pragma config JTAGEN = OFF             // JTAG Enable (JTAG Disabled)
#pragma config ICESEL = ICS_PGx1        // ICE/ICD Comm Channel Select (Communicate on PGEC1/PGED1)
#pragma config PWP = OFF                // Program Flash Write Protect (Disable)
#pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP = OFF                 // Code Protect (Protection Disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

void init_gpio_and_led(void)
{
    // analog vs digital
    ANSELA = 0x0001;    // all pins in digital mode except RA0
                        // A0 : TESTBENCH_REV
    ANSELB = 0x0000;    // [OK] all pins in digital mode
                        
    // DUTs
    CMD_DUT1_9V_OUT;
    CMD_DUT2_9V_OUT;
    CMD_DUT3_9V_OUT;

    CMD_DUT1_9V_CLR;  
    CMD_DUT2_9V_CLR;
    CMD_DUT3_9V_CLR;

    LED_DUT1_OUT;
    LED_DUT2_OUT;
    LED_DUT3_OUT;

    LED_DUT1_CLR;   
    LED_DUT2_CLR;
    LED_DUT3_CLR;
    
    DUT1_DETECT_IN;
    DUT2_DETECT_IN;
    DUT3_DETECT_IN;
            
    // LEDs
    LED0_OUT;
    LED1_OUT;
    LED2_OUT;

    LED0_CLR;
    LED1_CLR;
    LED2_CLR;

    // Test points
    TPO_OUT;
    TPV_OUT;

    TPO_CLR;
    TPV_CLR;

    // Pushbuttons
    SW0_IN;
    SW1_IN;
}

void init_timer1(void)
{
    T1CON = 0;
    PR1 = TIMER1_PERIOD-1;
    T1CONbits.TCKPS = 1;    // CLK div8
    IEC0bits.T1IE = 1;      // T1 IRQ on
    T1CONbits.ON = 1;       // T1 on  
}

void init_pps(void)
{
    // Unlock system for PPS configuration 
    SYSKEY = 0x00000000;
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;
    CFGCONbits.IOLOCK = 0;

    // PPS Input Remapping 
    U1RXR = 1;      // U1RX = PB6

    // PPS Output Remapping
    RPB7R = 1;      // PB7 = U1TX

    // Lock back the system after PPS configuration 
    CFGCONbits.IOLOCK = 1;
    SYSKEY = 0x00000000;
}

void init_clocks(void)
{
    // unlock system for clock configuration 
    SYSKEY = 0x00000000;
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;

    /* Peripheral Module Disable Configuration */
    PMD1 = 0x1101;
    PMD2 = 0x7;
    PMD3 = 0x1f001f;
    PMD4 = 0x1e;
    
//    PMD5bits.U1MD = 0;
    
    //PMD5 = 0x1030202;

    //PMD5bits.U1MD = 0;
    //PMD5bits.SPI1MD = 0;
    //PMD5bits.I2C1MD = 0;
    PMD6 = 0x10001;

    // Lock system since done with clock configuration 
    SYSKEY = 0x33333333;
}

void board_init(void)
{
    __builtin_disable_interrupts();

    init_clocks();

    /* Configure KSEG0 as cacheable memory. This is needed for Prefetch Buffer */
    __builtin_mtc0(16, 0,(__builtin_mfc0(16, 0) | 0x3));

    /* Set the SRAM wait states to zero */
    BMXCONbits.BMXWSDRM = 0;

    init_timer1();
    init_pps();
    init_uart1();
    init_gpio_and_led();
    init_button();
    init_irq();

    __builtin_enable_interrupts();
}

typedef struct {
    uint32_t current_state;
} testbench_state_t;

enum testbench_state_list {
    STARTUP = 0,                // must begin at 0
    WAITING_FOR_START,
    DUT1_STARTED,
    DUT1_FINISHED,
    DUT2_STARTED,
    DUT2_FINISHED,
    DUT3_STARTED,
    DUT3_FINISHED
};


int8_t count = 0;
testbench_state_t TESTBENCH;

void next_state(uint32_t next_state)
{
    // general rule
    TESTBENCH.current_state = next_state;
}

void update_pedal_state(void)
{
    switch ( TESTBENCH.current_state )
    {
        case STARTUP:
        {
            char c = uart1_getc();
            if ( c == 'i' )
            {
                console_printf("\r\nWAITING FOR DUT\r\n");
                next_state(WAITING_FOR_START);
            }
        }
        break;
    
        case WAITING_FOR_START:
        {
            
            LED0_CLR;
            LED_DUT1_CLR;
            LED1_CLR;
            LED_DUT2_CLR;  
            LED2_CLR;
            LED_DUT3_CLR;

            CMD_DUT1_9V_CLR;
            CMD_DUT2_9V_CLR;
            CMD_DUT3_9V_CLR;
            
            g_flag_dut1_sequence_finished = 0;
            g_flag_dut2_sequence_finished = 0;
            g_flag_dut3_sequence_finished = 0;
                
            char c = uart1_getc();
            if ( c == 's' && DUT1_DETECT_PIN == 0 )
            {
                console_printf("\r\nDUT1 STARTED\r\n");
                next_state(DUT1_STARTED);
            }
            
//            if ( BT[DUT1].status == SHORT_PRESSED )
//            {
//                console_printf("\r\nDUT1 STARTED\r\n");
//                next_state(DUT1_STARTED);
//            }
        }
        break;
    
        case DUT1_STARTED:
        {    
            LED0_SET;
            LED_DUT1_SET;
           
            CMD_DUT1_9V_SET;
            
            char c = uart1_getc();
            if ( c == 'e' )
            {
                g_flag_dut1_sequence_finished = 0;
                console_printf("\r\nDUT1 FINISHED\r\n");
                next_state(DUT1_FINISHED);
            }
                     
//            if ( BT[SW1].status == SHORT_PRESSED )  // g_flag_dut1_sequence_finished
//            {
//                g_flag_dut1_sequence_finished = 0;
//                console_printf("\r\nDUT1 FINISHED\r\n");
//                next_state(DUT1_FINISHED);
//            }
        }
        break;
            
        case DUT1_FINISHED:
        {
            LED0_CLR;
            LED_DUT1_CLR;
           
            CMD_DUT1_9V_CLR;
            delay_ms(50);
            
            char c = uart1_getc();
            if ( c == 's' && DUT2_DETECT_PIN == 0)
            {
                console_printf("\r\nDUT2 STARTED\r\n");
                next_state(DUT2_STARTED);
            }
                        
//            if ( BT[DUT2].status == SHORT_PRESSED )
//            {
//                console_printf("\r\nDUT2 STARTED\r\n");
//                next_state(DUT2_STARTED);
//            }
        }
        break;
    
        case DUT2_STARTED:
        {   
            LED1_SET;
            LED_DUT2_SET;
           
            CMD_DUT2_9V_SET;
            
            char c = uart1_getc();
            if ( c == 'e' )
            {
                g_flag_dut2_sequence_finished = 0;
                console_printf("\r\nDUT2 FINISHED\r\n");
                next_state(DUT2_FINISHED);
            }
        
//            if ( BT[SW1].status == SHORT_PRESSED )  // g_flag_dut2_sequence_finished
//            {
//                g_flag_dut2_sequence_finished = 0;
//                console_printf("\r\nDUT2 FINISHED\r\n");
//                next_state(DUT2_FINISHED);
//            }
        }
        break;
            
        case DUT2_FINISHED:
        {    
            LED1_CLR;
            LED_DUT2_CLR;
           
            CMD_DUT2_9V_CLR;
            delay_ms(50);
            
            char c = uart1_getc();
            if ( c == 's' && DUT3_DETECT_PIN == 0)
            {
                console_printf("\r\nDUT3 STARTED\r\n");
                next_state(DUT3_STARTED);
            }
                        
//            if ( BT[DUT3].status == SHORT_PRESSED )
//            {
//                console_printf("\r\nDUT3 STARTED\r\n");
//                next_state(DUT3_STARTED);
//            }
        }
        break;
            
        case DUT3_STARTED:
        {
            LED2_SET;
            LED_DUT3_SET;
           
            CMD_DUT3_9V_SET;
            
            char c = uart1_getc();
            if ( c == 'e' )
            {
                g_flag_dut3_sequence_finished = 0;
                console_printf("\r\nDUT3 FINISHED\r\n");
                next_state(DUT3_FINISHED);
            }
                        
//            if ( BT[SW1].status == SHORT_PRESSED )  // g_flag_dut3_sequence_finished
//            {
//                g_flag_dut3_sequence_finished = 0;
//                console_printf("\r\nDUT3 FINISHED\r\n");
//                next_state(DUT3_FINISHED);
//            }
        }
        break;
            
        case DUT3_FINISHED:
            
            LED2_CLR;
            LED_DUT3_CLR;
           
            CMD_DUT3_9V_CLR;
            delay_ms(50);
            
            console_printf("\r\nWAITING FOR DUT\r\n");
            next_state(WAITING_FOR_START);
            
            break;   
    }
}
int main( void )
{
    board_init();
    delay_ms(10);
   
    console_printf("\r\nOVTR\r\n");
    
    LED0_CLR;
    LED_DUT1_CLR;
    LED1_CLR;
    LED_DUT2_CLR;
    LED2_CLR;
    LED_DUT3_CLR;

    CMD_DUT1_9V_CLR;
    CMD_DUT2_9V_CLR;
    CMD_DUT3_9V_CLR;

    g_flag_dut1_sequence_finished = 0;
    g_flag_dut2_sequence_finished = 0;
    g_flag_dut3_sequence_finished = 0;
            
    TESTBENCH.current_state = STARTUP;
    
    while ( 1 )
    {       
        get_all_buttons_state();
        //uart1_basic_loopback();
        
        update_pedal_state();

        if ( TRIGGER_10MS_TASKS )
        {
            TRIGGER_10MS_TASKS = 0;
        }

        if ( TRIGGER_100MS_TASKS )
        {
            TRIGGER_100MS_TASKS = 0;
        }
     
//   // State machine handling
//        
//        // Wait for dut 1 detect
//        if ( BT[DUT1].status == SHORT_PRESSED )
//        {
//            //console_printf("\r\nDUT1 DETECTED\r\n");
//
//            LED0_SET;
//            LED_DUT1_SET;
//            LED1_CLR;
//            LED_DUT2_CLR;
//            LED2_CLR;
//            LED_DUT3_CLR;
//
//            CMD_DUT2_9V_CLR;
//            CMD_DUT3_9V_CLR;
//            delay_ms(250);
//            CMD_DUT1_9V_SET;
//        }
//        
//        // Wait for dut 2 detect and ack end of sequence dut 1
//        if ( BT[DUT2].status == SHORT_PRESSED )
//        {
//            //console_printf("\r\nDUT2 DETECTED\r\n");
//            
//            LED0_CLR;
//            LED_DUT1_CLR;
//            LED1_SET;
//            LED_DUT2_SET;
//            LED2_CLR;
//            LED_DUT3_CLR;
//
//            CMD_DUT1_9V_CLR;
//            CMD_DUT3_9V_CLR;
//            delay_ms(250);
//            CMD_DUT2_9V_SET;
//        }
//        
//        // Wait for dut 3 detect and ack end of sequence dut 2
//        if ( BT[DUT3].status == SHORT_PRESSED )
//        {
//            //console_printf("\r\nDUT3 DETECTED\r\n");
//            
//            LED0_CLR;
//            LED_DUT1_CLR;
//            LED1_CLR;
//            LED_DUT2_CLR;
//            LED2_SET;
//            LED_DUT3_SET;
//           
//            CMD_DUT1_9V_CLR;
//            CMD_DUT2_9V_CLR;
//            delay_ms(250);
//            CMD_DUT3_9V_SET;
//        }
    }
    return ( -1 );
}