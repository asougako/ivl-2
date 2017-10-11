
#include "chasy.h"
char    sec_tier = 0;

void        system_unlock(void)
{
    SYSKEY = 0x0;
    SYSKEY = 0xaa996655;
    SYSKEY = 0x556699aa;
}

void        system_lock(void)
{
    SYSKEY = 0x0;
}

void    init_clock()
{
    /*SOSC*/
    system_unlock();
    LATBbits.LATB4 = 1;             //hight
    TRISAbits.TRISA4 = 1;           //Input
    system_unlock();
    OSCCONbits.SOSCEN = 1;
    while (!(OSCCONbits.SOSCRDY));

    
    /*RTCC*/
    RTCCON = 0x0000;                //Disable RTCC
    while(RTCCON & 0x0040);         //Wait for RTCC is disable
    RTCCONSET = 0x0089;             //Allow user to write in RTC value registers
                                    //Clock output (1Hz) on RB3
    RTCCONbits.CAL = 0b0000000000;  //Calibration: 0 pulse per minutes
    RTCTIME = 0x00;                 //set RTCC time to 0
    RTCCONSET = 0x8000;             //RTCC ON
    while(!(RTCCON & 0x0040))       //Wait for RTCC is enable
    system_lock();
    
    return;
}



        