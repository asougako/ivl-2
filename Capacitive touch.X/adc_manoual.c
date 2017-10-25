
#include "chasy.h"

void            init_manual_adc(void)
{
	//ADC
        AD1CON1 = 0;                //[ON|FRZ|SIDL|FORM|SSRC|CLRASAM|ASAM|SAMP|DONE]
	AD1CON2 = 0;                //[VCFG|OFFCAL|CSCNA|BUFS|SMPI|BUFM|ALTS]
	AD1CON3 = 1;                //[ADRC|SAMC|ADCS = 0b10]
	AD1CSSL = 0;                //SKIP ANx input scan
	// ADC1 Interrupt
        IFS0bits.AD1IF = 0;
        IPC5bits.AD1IP = 4;
        IEC0bits.AD1IE = 0;
	// Activate ADC
        AD1CON1bits.ON = 1;
}