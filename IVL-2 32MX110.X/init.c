
#include "chasy.h"

void    init_gpio(void)
{
    //All pins set to DIGITAL OUTPUT LOW
    ANSELA = 0x00;
    ANSELB = 0x00;
    LATA = 0x00;
    LATB = 0x00;
    TRISA = 0x00;
    TRISB = 0x00;
    return;
}

void    init_timer(void)
{
    T1CONCLR = 0xffff;
    T2CONCLR = 0xffff;
    T3CONCLR = 0xffff;
    T4CONCLR = 0xffff;
    T5CONCLR = 0xffff;
    return;
}

void    init_interrupt(void)
{
    asm("di");
    INTCONSET = 0x5000;     //Freez in debug
                            //Multi vector
    asm("ei");
    return;
}

void    init(void)
{
    init_gpio();
    init_timer();
    init_interrupt();
    return;
}
