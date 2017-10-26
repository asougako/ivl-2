
#include "chasy.h"

/* Pins configuration
    RB6 -> OC4 -> RED
    RB7 -> OC1 -> GREEN
    RB8 -> OC2 -> BLUE
*/

void    init_led()
{
    /*Peripheral pin select*/
    RPB6R       = 0x05;         // Pin B6 mapped to Output Compare 3
    RPB7R       = 0x05;         // Pin B7 mapped to Output Compare 1
    RPB8R       = 0x05;         // Pin B8 mapped to Output Compare 2

    /*Output compare as PWM*/
    //OCx CLR
    OC1CON      = 0x0000;
    OC2CON      = 0x0000;
    OC4CON      = 0x0000;
    //TMR2 config
    T2CON       = 0x0010;       // TCKPS = 1:2
    TMR2        = 0x0000;       // CLR TMR2
    PR2         = 10000 - 1;    // PBCLK @ 20MHz, TCKPS = 1:2 >> PWM = 1KHz
    //Duty cycle
    OC1R        = 0;            //50%
    OC1RS       = 0;            //50%
    OC2R        = 0;            //0%
    OC2RS       = 0;            //0%
    OC4R        = 0;            //0%
    OC4RS       = 0;            //0%
    //OCx config
    OC1CON      = 0x0006;       // OC OFF, 16bits, TMR2, Mode PWM, fault pin disabled
    OC2CON      = 0x0006;       // OC OFF, 16bits, TMR2, Mode PWM, fault pin disabled
    OC4CON      = 0x0006;       // OC OFF, 16bits, TMR2, Mode PWM, fault pin disabled
    //Activate TMR2
    T2CONSET     = 0x8000;      // TMR2 ON
    //Activate OCx
    OC1CONSET   = 0x8000;       // OC1 ON
    OC2CONSET   = 0x8000;       // OC1 ON
    OC4CONSET   = 0x8000;       // OC1 ON




    /*TMR 1 used to change LED colors*/
    T1CON = 0;
    T1CON = 0x0030;     //TCKPS = 1:256
    TMR1 = 0;
    PR1 = 26041;        //PBCLK @ 20MHz >> 1:256 = 78125Hz >> PR1 @ 26041 = 3Hz

    //T1 interrupt
    IEC0bits.T1IE = 1;
    IPC1bits.T1IP = 4;
    IPC1bits.T1IS = 0;
    IFS0bits.T1IF = 0;

    T1CONSET = 0x8000;

    return;
}

void    led_set(void)
{
    OC1RS = green_duty;
    OC2RS = blue_duty;
    OC4RS = red_duty;
    return;
}

unsigned int    led_index = 0;
void    __attribute((interrupt(IPL4AUTO), vector(4)))change_led_color(void)
{
    IFS0bits.T1IF = 0;      //CLR TMR interrupt flag
    /*change color*/

    switch (led_index)
    {
        case 0:
            blue_duty += cycle_speed * 10;
            if (blue_duty > 10000)
            {
                blue_duty = 10000;
                led_index++;
            }
            break;
        case 1:
            green_duty -= cycle_speed * 10;
            if (green_duty < 0)
            {
                green_duty = 0;
                led_index++;
            }
            break;
        case 2:
            red_duty += cycle_speed * 10;
            if (red_duty > 10000)
            {
                red_duty = 10000;
                led_index++;
            }
            break;
        case 3:
            blue_duty -= cycle_speed * 10;
            if (blue_duty < 0)
            {
                blue_duty = 0;
                led_index++;
            }
            break;
        case 4:
            green_duty += cycle_speed * 10;
            if (green_duty > 10000)
            {
                green_duty = 10000;
                led_index++;
            }
            break;
        case 5:
            red_duty -= cycle_speed * 10;
            if (red_duty < 0)
            {
                red_duty = 0;
                led_index = 0;
            }
            break;
    }
    led_set();
}
