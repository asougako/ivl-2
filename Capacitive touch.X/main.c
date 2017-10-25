
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

#include <xc.h>
#include "chasy.h"

#define main_loop while(1)
#define loop_delay 100000

int g_sec01 = 0;
int g_sec10 = 0;
int g_min01 = 0;
int g_min10 = 0;

void    main(void)
{
<<<<<<< HEAD
    uint16_t    touch_val = 0;
    uint16_t     touch_limit = 840;
    
    init();
    init_delay();

    init_touch();

    main_loop
    {
        touch_val = get_touch();
        if (touch_val < touch_limit)
        {
            LATBSET = 0x0020;
            touch_limit = 860;
        }
        else
        {
            LATBCLR = 0x0020;
            touch_limit = 840;
        }
=======
    init();
    init_delay();

    //init red led on RB5
    LATBSET = 0x0020;
    micro_delay(500000);
    LATBCLR = 0x0020;

    /*init capacitive touch on RB15(AN9)*/
    /*  CTMU*/
    CTMUCON = 0;
    CTMUCONbits.IRNG    = 0b00;         //Base current source range
    CTMUCONbits.ITRIM   = 0b000000;     //Nominal current source trim
    CTMUCONbits.CTTRIG  = 1;            //Auto trigger ADC @ second edge
    /*AD1*/
    TRISBbits.TRISB15   = 1;            //RB15 as inout
    ANSELBbits.ANSB15   = 1;            //RB15 as analog pin
    AD1CHSbits.CH0SA    = 0b1001;       //ADC1 positive input: chanel 9
    AD1CHSbits.CH0NA    = 0;            //ADC1 negative input: Vrefl
    AD1CON1bits.FORM    = 0b000;        //Data output: 16 bits integer
    AD1CON1bits.SSRC    = 0b000;//0b011        //Conversion triger source: CTMU
    AD1CON2bits.VCFG    = 0b000;        //Voltage ref source: AVdd, AVss
    AD1CON2bits.CSCNA   = 0;            //Scan mode: off
    AD1CON2bits.SMPI    = 0b0000;       //NB of conversions per interrupt: 1
    AD1CON2bits.BUFM    = 0;            //Buffer fill mode: 16 * 1word buffer
    AD1CON2bits.ALTS    = 0;            //MUX: A
    AD1CON3bits.ADRC    = 0;            //Conversion clock: PBCLK
    AD1CON3bits.SAMC    = 0b00000;      //Auto-sample time: Tad
    AD1CON3bits.ADCS    = 0;            //Conversion clock
    AD1CON1bits.ON      = 0;            //Enable ADC

    main_loop
    {
        int ctmu_val = 0;
    /*CTMU*/
//        CTMUCONbits.ON          = 0;    //Disable CTMU
//        CTMUCONbits.IDISSEN     = 1;    //Discharge circuit
//        micro_delay(1000);              //Wait 1ms
//        CTMUCONbits.IDISSEN     = 0;    //Stop discharge
//        CTMUCONbits.EDG1STAT    = 0;    //Reset edge1 status
//        CTMUCONbits.EDG2STAT    = 0;    //Reset edge2 status
//        CTMUCONbits.EDGEN       = 1;    //Unblock edge inputs
//        CTMUCONbits.ON          = 1;    //Enable CTMU

        /*ADC*/
        AD1CON1bits.ON          = 1;    //Enable ADC
        AD1CON1bits.SAMP        = 1;    //Start sampling
        micro_delay(10);            //
        AD1CON1bits.SAMP        = 0;    //Stop sampling, start conversion
        while (!AD1CON1bits.DONE);
        ctmu_val = (ADC1BUF0 * 330) / 1023;
        micro_delay(1);
>>>>>>> e7a9ebca2a057ed6cd91ad20ccdb3a79f145d8e5
    }
}
