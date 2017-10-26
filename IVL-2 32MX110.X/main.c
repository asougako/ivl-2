
// PIC32MX110F016B Configuration Bit Settings

// 'C' source line config statements

// DEVCFG3
// USERID = No Setting
#pragma config PMDL1WAY = ON            // Peripheral Module Disable Configuration (Allow only one reconfiguration)
#pragma config IOL1WAY = ON             // Peripheral Pin Select Configuration (Allow only one reconfiguration)

// DEVCFG2
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
#pragma config FPLLMUL = MUL_20         // PLL Multiplier (20x Multiplier)
#pragma config FPLLODIV = DIV_2         // System PLL Output Clock Divider (PLL Divide by 2)

// DEVCFG1
#pragma config FNOSC = FRCPLL           // Oscillator Selection Bits (Fast RC Osc with PLL)
#pragma config FSOSCEN = ON             // Secondary Oscillator Enable (Disabled)
#pragma config IESO = ON                // Internal/External Switch Over (Enabled)
#pragma config POSCMOD = OFF            // Primary Oscillator Configuration (Primary osc disabled)
#pragma config OSCIOFNC = OFF           // CLKO Output Signal Active on the OSCO Pin (Disabled)
#pragma config FPBDIV = DIV_2           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/2)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
#pragma config WDTPS = PS1048576        // Watchdog Timer Postscaler (1:1048576)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable (Watchdog Timer is in Non-Window Mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))
#pragma config FWDTWINSZ = WINSZ_25     // Watchdog Timer Window Size (Window Size is 25%)

// DEVCFG0
#pragma config JTAGEN = OFF             // JTAG Enable (JTAG Disabled)
#pragma config ICESEL = ICS_PGx1        // ICE/ICD Comm Channel Select (Communicate on PGEC1/PGED1)
#pragma config PWP = OFF                // Program Flash Write Protect (Disable)
#pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP = OFF                 // Code Protect (Protection Disabled)

//CPU @ 40MHz
//PBCLK @ 20MHz

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

/*TIMERS:
 * TMR1 
 * TMR2 
 * TMR3 ivl2
 * TMR4 touch
 * TMR5 delay
 */

#include <xc.h>
#include "chasy.h"

#define main_loop while(1)

#define touch_moins 0   //AN0
#define touch_plus  1   //AN1
#define touch_mode  4   //AN4
#define touch_moins_trig 800   //AN0
#define touch_plus_trig  500   //AN1
#define touch_mode_trig  950   //AN4
#define touch_delay      30000 //30ms between touchs

#define mode_set_min    0
#define mode_set_hr     1
#define mode_set_cycle  2
#define mode_set_speed  3
#define mode_set_red    4
#define mode_set_green  5
#define mode_set_blue   6

int cycle_mode = 0;
int cycle_speed = 42;
int red_duty = 0;
int green_duty = 10000;
int blue_duty = 0;

void    main(void)
{
    int touch_val1 = 0;
    int touch_val2 = 0;
    int touch_val3 = 0;
    uint8_t ivl_mode = 0;
    
    init();
    init_delay();
    init_led();
    init_clock();
    init_ivl2();
    init_touch();

    led_set();     //update led color       

    /*default digits display*/
    ivl2_set_digit(0, 8);
    ivl2_set_digit(1, 8);
    ivl2_set_digit(2, 8);
    ivl2_set_digit(3, 8);

    //test
    time_sync_update(0x13370000L);
    
    main_loop
    {
        //update time       >> Use RTCC interrupt (1min alarm?)
        switch (ivl_mode)
        {
            case 0:
                ivl2_set_digit(0, RTCTIMEbits.HR10);
                ivl2_set_digit(1, RTCTIMEbits.HR01);
                ivl2_set_digit(2, RTCTIMEbits.MIN10);
                ivl2_set_digit(3, RTCTIMEbits.MIN01);
                break;
            case 1:
                ivl2_set_digit(0, RTCTIMEbits.HR10);
                ivl2_set_digit(1, RTCTIMEbits.HR01);
                ivl2_set_digit(2, RTCTIMEbits.MIN10);
                ivl2_set_digit(3, RTCTIMEbits.MIN01);
                break;
            case 2:
                ivl2_set_digit(0, 11);
                ivl2_set_digit(1, 10);
                ivl2_set_digit(2, 10);
                ivl2_set_digit(3, cycle_mode);
                break;
            case 3:
                ivl2_set_digit(0, 12);
                ivl2_set_digit(1, cycle_speed / 100);
                ivl2_set_digit(2, (cycle_speed / 10 ) % 10);
                ivl2_set_digit(3, cycle_speed % 10);
                break;
            case 4:
                ivl2_set_digit(0, 13);
                ivl2_set_digit(1, red_duty / 10000);
                ivl2_set_digit(2, (red_duty / 1000) % 10);
                ivl2_set_digit(3, (red_duty / 100) % 10);
                break;
            case 5:
                ivl2_set_digit(0, 14);
                ivl2_set_digit(1, green_duty / 10000);
                ivl2_set_digit(2, (green_duty / 1000) % 10);
                ivl2_set_digit(3, (green_duty / 100) % 10);
                break;
            case 6:
                ivl2_set_digit(0, 15);
                ivl2_set_digit(1, blue_duty / 10000);
                ivl2_set_digit(2, (blue_duty / 1000) % 10);
                ivl2_set_digit(3, (blue_duty / 100) % 10);
                break;                
            default:
                break;
        }
        
        //check capacitive input
        if ((touch_val1 = get_touch(touch_moins)) < touch_moins_trig)
        {
            switch (ivl_mode)
            {
                case mode_set_min:
                    time_min_decrement();
                    break;
                case mode_set_hr:
                    time_hr_decrement();
                    break;
                case mode_set_cycle:
                    cycle_mode--;
                    if (cycle_mode < 0)
                        cycle_mode = 2;
                    switch(cycle_mode)
                    {
                        case 0:
                            T1CONSET = 0x8000;
                            break;
                        case 1:
                            T1CONCLR = 0x8000;
                            break;
                        case 2:
                            T1CONCLR = 0x8000;
                            red_duty = 0;
                            green_duty = 0;
                            blue_duty = 0;
                            led_set();
                            break;
                        default:
                            break;
                    }
                    break;
                case mode_set_speed:
                    cycle_speed--;
                    if (cycle_speed < 0)
                        cycle_speed = 0;
                    break;
                case mode_set_red:
                    red_duty -= 100;
                    if (red_duty < 0)
                        red_duty = 0;
                    led_set();
                    break;
                case mode_set_green:
                    green_duty -= 100;
                    if (green_duty < 0)
                        green_duty = 0;
                    led_set();
                    break;
                case mode_set_blue:
                    blue_duty -= 100;
                    if (blue_duty < 0)
                        blue_duty = 0;
                    led_set();
                    break;
                default:
                    break;
            }
            micro_delay(touch_delay);
        }
        if ((touch_val2 = get_touch(touch_plus)) < touch_plus_trig)
        {
            switch (ivl_mode)
            {
                case mode_set_min:
                    time_min_increment();
                    break;
                case mode_set_hr:
                    time_hr_increment();
                    break;
                case mode_set_cycle:
                    cycle_mode++;
                    if (cycle_mode > 2)
                        cycle_mode = 0;
                    switch(cycle_mode)
                    {
                        case 0:
                            T1CONSET = 0x8000;
                            break;
                        case 1:
                            T1CONCLR = 0x8000;
                            break;
                        case 2:
                            T1CONCLR = 0x8000;
                            red_duty = 0;
                            green_duty = 0;
                            blue_duty = 0;
                            led_set();
                            break;
                        default:
                            break;
                    }
                    break;
                case mode_set_speed:
                    cycle_speed++;
                    if (cycle_speed > 100)
                        cycle_speed = 100;
                    break;
                case mode_set_red:
                    red_duty += 100;
                    if (red_duty > 10000)
                        red_duty = 10000;
                    led_set();
                    break;
                case mode_set_green:
                    green_duty += 100;
                    if (green_duty > 10000)
                        green_duty = 10000;
                    led_set();
                    break;
                case mode_set_blue:
                    blue_duty += 100;
                    if (blue_duty > 10000)
                        blue_duty = 10000;
                    led_set();
                    break;
                default:
                    break;
            }
            micro_delay(touch_delay);
        }
        if ((touch_val3 = get_touch(touch_mode)) < touch_mode_trig)
        {
            ivl_mode++;
            if (ivl_mode > 6 || (ivl_mode > 3 && cycle_mode != 1))       //6 modes
                ivl_mode = 0;
            
            switch (ivl_mode)
            {
                case 0:             //Set min
                    ivl2_set_digit(0, RTCTIMEbits.HR10);
                    ivl2_set_digit(1, RTCTIMEbits.HR01);
                    ivl2_set_digit(2, RTCTIMEbits.MIN10);
                    ivl2_set_digit(3, RTCTIMEbits.MIN01);
                    blink_min();
                    break;
                case 1:             //Set hour
                    blink_hr();
                    break;
                default:
                    break;
            }
            micro_delay(touch_delay * 2);
        }
        micro_delay(1);
    }
}
