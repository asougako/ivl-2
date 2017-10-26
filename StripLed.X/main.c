
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

/*
 * Data in on RB6
 */

#include "chasy.h"

#define gap 5
void    main(void)
{
    init();
    init_delay();
    init_strip();    

    uint8_t i_led;
    uint8_t step[8] = {0,1,2,3,4,5,0,1};
    
    
    set_led(0, 255, 000, 000);
    set_led(1, 255, 255, 000);
    set_led(2, 000, 255, 000);
    set_led(3, 000, 255, 255);
    set_led(4, 000, 000, 255);
    set_led(5, 255, 000, 255);
    set_led(6, 255, 000, 000);
    set_led(7, 255, 255, 000);
    
    main_loop
    {
        micro_delay(10000);
        for(i_led = 0; i_led < 8; i_led++)
        {
            switch (step[i_led])
            {
                case 0:
                    set_led(i_led, strip_color[i_led].r, strip_color[i_led].g + gap, strip_color[i_led].b);
                    if (strip_color[i_led].g >= 255)
                    {
                        strip_color[i_led].g = 255;
                        step[i_led]++;
                        if (step[i_led] > 5)
                            step[i_led] = 0;
                    }
                    break;
                case 1:
                    set_led(i_led, strip_color[i_led].r - gap, strip_color[i_led].g, strip_color[i_led].b);
                    if (strip_color[i_led].r <= 0)
                    {
                        strip_color[i_led].r = 0;
                        step[i_led]++;
                        if (step[i_led] > 5)
                            step[i_led] = 0;
                    }
                    break;
                case 2:
                    set_led(i_led, strip_color[i_led].r, strip_color[i_led].g, strip_color[i_led].b + gap);
                    if (strip_color[i_led].b >= 255)
                    {
                        strip_color[i_led].b = 255;
                        step[i_led]++;
                        if (step[i_led] > 5)
                            step[i_led] = 0;
                    }
                    break;
                case 3:
                    set_led(i_led, strip_color[i_led].r, strip_color[i_led].g - gap, strip_color[i_led].b);
                    if (strip_color[i_led].g <= 0)
                    {
                        strip_color[i_led].g = 0;
                        step[i_led]++;
                        if (step[i_led] > 5)
                            step[i_led] = 0;
                    }
                    break;
                case 4:
                    set_led(i_led, strip_color[i_led].r + gap, strip_color[i_led].g, strip_color[i_led].b);
                    if (strip_color[i_led].r >= 255)
                    {
                        strip_color[i_led].r = 255;
                        step[i_led]++;
                        if (step[i_led] > 5)
                            step[i_led] = 0;
                    }
                    break;
                case 5:
                    set_led(i_led, strip_color[i_led].r, strip_color[i_led].g, strip_color[i_led].b - gap);
                    if (strip_color[i_led].b <= 0)
                    {
                        strip_color[i_led].b = 0;
                        step[i_led]++;
                        if (step[i_led] > 5)
                            step[i_led] = 0;
                    }
                    break;
                default:
                    break;
            }
        }
        
        set_strip();
    }
}
