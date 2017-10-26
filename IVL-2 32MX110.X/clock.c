
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

void    time_sync_update(unsigned long new_time)
{
    asm volatile("di");                 //Disable interrupts
    while (RTCCONbits.RTCSYNC != 0);    //Wait Sync window
    RTCTIME = new_time;                 //Update time
    asm volatile("ei");                 //Enable interrupts
    return;
}

void    time_min_increment(void)
{
    RTCCONCLR = 0x8000;             //Disable RTCC
    while (RTCCONbits.RTCCLKON);    //Wait RTCC off
    if (RTCTIMEbits.MIN01 == 9)
    {
        RTCTIMEbits.MIN01 = 0;
        if (RTCTIMEbits.MIN10 == 5)
        {
            RTCTIMEbits.MIN10 = 0;
            if (RTCTIMEbits.HR01 == 9 && RTCTIMEbits.HR10 < 2)
            {
                RTCTIMEbits.HR01 = 0;
                RTCTIMEbits.HR10++;
            }
            else if (RTCTIMEbits.HR01 >= 3)
            {
                RTCTIME = (0x00000000L);
            }
            else
            {
                RTCTIMEbits.HR01++;
            }
        }
        else
        {
            RTCTIMEbits.MIN10++;
        }
    }
    else
    {
        RTCTIMEbits.MIN01++;
    }
    RTCCONSET = 0x8000;             //Enable RTCC
    while (!RTCCONbits.RTCCLKON);    //Wait RTCC on
}
    
void    time_min_decrement(void)
{
    RTCCONCLR = 0x8000;             //Disable RTCC
    while (RTCCONbits.RTCCLKON);    //Wait RTCC off
    if (RTCTIMEbits.MIN01 == 0)
    {
        RTCTIMEbits.MIN01 = 9;
        if (RTCTIMEbits.MIN10 == 0x00)
        {
            RTCTIMEbits.MIN10 = 5;
            if (RTCTIMEbits.HR01 == 0 && RTCTIMEbits.HR10 > 0)
            {
                RTCTIMEbits.HR01 = 9;
                RTCTIMEbits.HR10--;
            }
            else if (RTCTIMEbits.HR01 == 0)
            {
                RTCTIME = (0x23590000L);
            }
            else
            {
                RTCTIMEbits.HR01--;
            }
        }
        else
        {
            RTCTIMEbits.MIN10--;
        }
    }
    else
    {
        RTCTIMEbits.MIN01--;
    }
    RTCCONSET = 0x8000;                 //Enable RTCC
    while (!RTCCONbits.RTCCLKON);       //Wait RTCC on    
}

void    time_hr_increment(void)
{
    RTCCONCLR = 0x8000;             //Disable RTCC
    while (RTCCONbits.RTCCLKON);    //Wait RTCC off
    if (RTCTIMEbits.HR01 == 9 && RTCTIMEbits.HR10 < 2)
    {
        RTCTIMEbits.HR01 = 0;
        RTCTIMEbits.HR10++;
    }
    else if (RTCTIMEbits.HR01 >= 3 && RTCTIMEbits.HR10 >= 2)
    {
        RTCTIME = 0x00000000 + (RTCTIME & 0x00ff0000);
//        RTCTIMEbits.HR01 = (0x0);
//        RTCTIMEbits.HR10 = (0x0);
    }
    else
    {
        RTCTIMEbits.HR01++;
    }

    
    RTCCONSET = 0x8000;             //Enable RTCC
    while (!RTCCONbits.RTCCLKON);    //Wait RTCC on
}

void    time_hr_decrement(void)
{
    RTCCONCLR = 0x8000;             //Disable RTCC
    while (RTCCONbits.RTCCLKON);    //Wait RTCC off
    if (RTCTIMEbits.HR01 == 0 && RTCTIMEbits.HR10 > 0)
    {
        RTCTIMEbits.HR01 = 9;
        RTCTIMEbits.HR10--;
    }
    else if (RTCTIMEbits.HR01 == 0 && RTCTIMEbits.HR10 == 0)
    {
        RTCTIME = 0x23000000 + (RTCTIME & 0x00ff0000);
//        RTCTIMEbits.HR10 = (0x2);
//        RTCTIMEbits.HR01 = (0x3);
    }
    else
    {
        RTCTIMEbits.HR01--;
    }
    RTCCONSET = 0x8000;                 //Enable RTCC
    while (!RTCCONbits.RTCCLKON);       //Wait RTCC on    
}
