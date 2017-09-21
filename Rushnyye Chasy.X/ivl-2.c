
#include "chasy.h"

//Constantes
#define nb_grids    5
#define nb_segments 7
#define pulse_delay 1
//ivl-2 pins values
#define ivl2_din    0x0100  //Data input
#define ivl2_clk    0x2000  //Shift data on rising edge
#define ivl2_load   0x4000  //Latch data, active high
#define ivl2_blank  0x8000  //Disable output on high
//Display values
uint8_t segment[4] = {0,0,0,0};             //Digit segments patern storage
uint8_t dots = 0;                           //Dots on/off status
uint8_t current_index = 0;                  //Store currently displayed digit
//Segments patterns
uint8_t num_to_seg[10] = {  0b01111110,     // 0
                            0b00010010,     // 1
                            0b01011101,     // 2
                            0b01011011,     // 3
                            0b00110011,     // 4
                            0b01101011,     // 5
                            0b01101111,     // 6
                            0b01010010,     // 7
                            0b01111111,     // 8
                            0b01111011};    // 9
//Grids pins value
uint8_t grid[4] = {         0b00000001,     //grid1
                            0b00000010,     //grid2
                            0b00001000,     //grid4
                            0b00010000};    //grid5

void    init_ivl2(void)
{
    //Timer2
    TMR2 = 0;           //Reset timer
    PR2 = 625;          //1milli seconde @ PBCLK = 20MHz, TCKPS = 1:32
    T2CONSET = 0x4040;  //Freeze in debug mode
                        //  TCKPS = 1:16

    //Timer 2 interrupt
    IFS0bits.T2IF = 0;  //Clear flag
    IPC2bits.T2IP = 5;  //Priority
    IPC2bits.T2IS = 0;  //Subpriority
    IEC0bits.T2IE = 1;  //Enable

    //Timer2
    T2CONSET = 0x8000;    //Enable
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
        LATBbits.LATB12 = (grid[index] >> i) & 0x01;
        ivl2_clk_pulse();
    }

    //Set segments
    for (i = 0; i < nb_segments; i++)
    {
        LATBbits.LATB12 = (segment[index] >> i) & 0x01;
        ivl2_clk_pulse();
    }
    ivl2_load_pulse();
    LATBCLR = ivl2_blank;
    return;
}

void    ivl2_print_dots(void)
{

}

void    __attribute__((interrupt(IPL5AUTO), vector(8)))refresh_digit(void)
{
    IFS0bits.T2IF = 0;
    TMR2 = 0;
    ivl2_print(current_index);  //print digit
    current_index++;
    if (current_index >= nb_grids - 1)
        current_index = 0;
    return;
}
