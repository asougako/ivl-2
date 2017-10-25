#include "chasy.h"

void    init_moisture()
{
//OLD SOIL SENSOR GPIO 26 RB15 CDTED6 AN 9
//SOIL SENSOR GPIO 23 RB12 AN 12
    TRISBbits.TRISB12 = 1;          //SET PIN at INPUT
    
//CTMU 1
    CTMUCON = 0;                    // RESET all
    CTMUCONbits.IRNG = 0b11;        // Current source trim at 55uA (I)
    
//TIMER
    TMR3 = 0;                       // Set Start to 0
    T3CONbits.TCKPS = 0;            // Set Prescale to 1
    PR3 = 10000;                    // 1mSec
    T3CONbits.ON = 1;               // Enable Timer

}

uint16_t get_moisture()
{
    uint16_t vctmu;
    uint16_t cap;

    vctmu = 0;
    TMR3 = 0;
    AD1CHSbits.CH0SA = 12;          // Select chanel AN12
    while (vctmu == 0)
    {
        CTMUCONbits.ON = 1;         // Turn On CTMU
        TMR3 = 0;                       // Set Start to 0
        while (TMR3 < 10000);         // wait 1ms
        AD1CON1bits.SAMP = 1;       // Begin manual sampling ADC
        CTMUCONbits.IDISSEN = 1;    // Discharge the connected circuit
        TMR3 = 0;                       // Set Start to 0
        while (TMR3 < 10000);         // wait 1ms
        CTMUCONbits.IDISSEN = 0;    // Stop manual sampling
        CTMUCONbits.EDG1STAT = 1;   // Begin charging circuit
        TMR3 = 0;                       // Set Start to 0
        while (TMR3 < 11);          // wait 1us (66% max charge)
        AD1CON1bits.SAMP = 0;       // End Sampling begin conversion
        CTMUCONbits.EDG1STAT = 0;   // Stop charging circuit
        while(!AD1CON1bits.DONE);   // wait ADC is done
        AD1CON1bits.DONE = 0;       // Clear flag
        vctmu = ADC1BUF0;           // get value from ADC
    }
    CTMUCONbits.ON = 0;             // CTMU off
    return (vctmu);
}
