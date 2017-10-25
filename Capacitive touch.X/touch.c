
#include "chasy.h"

void    init_touch(void)
{
    //init red led on RB5
    LATBSET = 0x0020;
    micro_delay(500000);
    LATBCLR = 0x0020;

    /*init capacitive touch on RB15(AN9)*/
    /*  GPIO*/
    TRISBbits.TRISB15   = 1;            //RB15 as inout
//    ANSELBbits.ANSB15   = 1;            //RB15 as analog pin
    /*  CTMU*/
    CTMUCON = 0;
    CTMUCONbits.IRNG    = 0b01;         //Base current source range x100 (55uA)
    CTMUCONbits.ITRIM   = 0b000000;     //Nominal current source trim
    CTMUCONbits.CTTRIG  = 0;            //Auto trigger ADC @ second edge
    /*AD1*/
    AD1CHSbits.CH0SA    = 9;       //ADC1 positive input: chanel 9
    AD1CHSbits.CH0NA    = 0;            //ADC1 negative input: Vrefl
    AD1CON1bits.FORM    = 0b000;        //Data output: 16 bits integer
    AD1CON1bits.SSRC    = 0b000;        //Conversion triger source: CLR SAMP bit
    AD1CON2bits.VCFG    = 0b000;        //Voltage ref source: AVdd, AVss
    AD1CON2bits.CSCNA   = 0;            //Scan mode: off
    AD1CON2bits.SMPI    = 0b0000;       //NB of conversions per interrupt: 1
    AD1CON2bits.BUFM    = 0;            //Buffer fill mode: 16 * 1word buffer
    AD1CON2bits.ALTS    = 0;            //MUX: A
    AD1CON3bits.ADRC    = 0;            //Conversion clock: PBCLK
    AD1CON3bits.SAMC    = 0b00000;      //Auto-sample time: Tad
    AD1CON3bits.ADCS    = 0x01;         //Conversion clock 4Tpb = Tad
    AD1CSSL = 0;                        //SKIP ANx input scan
    AD1CON1bits.ON      = 1;            //Enable ADC
    //TIMER
    TMR3 = 0;                       // Set Start to 0
    T3CONbits.TCKPS = 0;            // Set Prescale to 1
    PR3 = 0xffff;                    // 1mSec
    T3CONbits.ON = 1;               // Enable Timer
}

#define n 30
int     get_touch(void)
{
    int ctmu_val = 0;
    int count = 0;
    /*CTMU*/
//        CTMUCONbits.EDG1STAT    = 0;    //Reset edge1 status
//        CTMUCONbits.EDG2STAT    = 0;    //Reset edge2 status
//        CTMUCONbits.EDGEN       = 1;    //Unblock edge inputs

    CTMUCONbits.ON          = 1;    //Enable CTMU
    while (count < n)
    {
        micro_delay(1000);              //1ms ctmu warm up
        AD1CON1bits.SAMP        = 1;    //Start sampling
        CTMUCONbits.IDISSEN     = 1;    //Discharge circuit
        micro_delay(1000);              //Wait 1ms
        CTMUCONbits.IDISSEN     = 0;    //Stop discharge
        CTMUCONbits.EDG1STAT    = 1;    //Start charging
    //    micro_delay(1);
        TMR3 = 0;
        while(TMR3 < 3000);
        AD1CON1bits.SAMP        = 0;    //Stop sampling, start conversion
        CTMUCONbits.EDG1STAT    = 0;    //Reset edge2 status
        while (!AD1CON1bits.DONE);
        AD1CON1bits.DONE        = 0;    //CLR done flag
        ctmu_val += ADC1BUF0;
        count++;
    }
    CTMUCONbits.ON          = 0;    //Disable CTMU
    ctmu_val = ctmu_val / n;
    return(ctmu_val);
}