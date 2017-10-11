
#include "chasy.h"

#define PR_1us 20    //Cycle count for 1 micro sec @ 20 MHz

uint64_t    g_delay_count = 0;

void    init_delay(void)
{
    //Timer5
    TMR5 = 0;               //Reset timer
    PR5 = PR_1us;           //1 micro sec
    T5CONSET = 0x4000;      //Stoped in debug mode  //unimplemented on family 1xx/2xx
                            //  TCKPS 1:1

    //Timer5 interrupt
    IFS0bits.T5IF = 0;      //Flag down
    IPC5bits.T5IP = 3;      //Priority
    IPC5bits.T5IS = 0;      //Subpriority
    IEC0bits.T5IE = 1;      //Enable

    return;
}

void    __attribute__((interrupt(IPL3AUTO), vector(20)))delay_interrupt(void)
{
    IFS0bits.T5IF = 0;
    TMR5 = 0;
    g_delay_count++;
}

void    micro_delay(int delay)
{
    T5CONSET = 0x8000;              //TMR5 ON
    g_delay_count = 0;
    while (g_delay_count < delay);
    T5CONCLR = 0x8000;              //TMR5 ON
    return;
}
