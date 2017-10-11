
#include "chasy.h"

//Constantes
#define nb_grids    4
#define nb_segments 8
#define pulse_delay 1
//ivl-2 pins values
#define ivl2_din    0x8000  //Data input
#define ivl2_clk    0x2000  //Shift data on rising edge
#define ivl2_load   0x4000  //Latch data, active high
#define ivl2_blank  0x0200  //Disable output on high
//Display values
uint8_t segment[4] = {0,0,0,0};             //Digit segments patern storage
uint8_t dots = 0;                           //Dots on/off status
uint8_t current_index = 0;                  //Store currently displayed digit
//Segments patterns
uint8_t num_to_seg[10] = {  0b11111110,     // 0
                            0b01010000,     // 1
                            0b10110101,     // 2
                            0b01110101,     // 3
                            0b01011001,     // 4
                            0b01101101,     // 5
                            0b11101111,     // 6
                            0b01010100,     // 7
                            0b11111101,     // 8
                            0b01111101};    // 9
//Grids pins value
uint8_t grid[4] = {         0b00000001,     //grid1
                            0b00000010,     //grid2
                            0b00000100,     //grid4
                            0b00001000};    //grid5

void    init_ivl2(void)
{
    //Timer3
    TMR3 = 0;           //Reset timer
    PR3 = 625;          //1milli seconde @ PBCLK = 20MHz, TCKPS = 1:32
    T3CONSET = 0x4040;  //Freeze in debug mode
                        //  TCKPS = 1:16

    //Timer 3 interrupt
    IFS0bits.T3IF = 0;  //Clear flag
    IPC3bits.T3IP = 5;  //Priority
    IPC3bits.T3IS = 0;  //Subpriority
    IEC0bits.T3IE = 1;  //Enable

    //Timer3
    T3CONSET = 0x8000;    //Enable
}

void    ivl2_set_digit(int index, int val)
{
    segment[index] = num_to_seg[val];
    return;
}

void    ivl2_clk_pulse(void)
{
    LATBSET = ivl2_clk;
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    LATBCLR = ivl2_clk;
    return;
}

void    ivl2_load_pulse(void)
{
    LATBSET = ivl2_load;
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    LATBCLR = ivl2_load;
    return;
}

void    ivl2_print(int index)
{
    int i;

    LATBSET = ivl2_blank;
    //Set grid
    for (i = 0; i < nb_grids; i++)
    {
        LATBbits.LATB15 = (grid[index] >> i) & 0x01;
        ivl2_clk_pulse();
    }

    //Set segments
    for (i = 0; i < nb_segments; i++)
    {
        LATBbits.LATB15 = (segment[index] >> i) & 0x01;
        ivl2_clk_pulse();
    }
    ivl2_load_pulse();
    LATBCLR = ivl2_blank;
    return;
}

void    __attribute__((interrupt(IPL5AUTO), vector(12)))refresh_digit(void)
{
    IFS0bits.T3IF = 0;
    TMR3 = 0;
    ivl2_print(current_index);  //print digit
    current_index++;
    if (current_index >= nb_grids)
        current_index = 0;
    return;
}
