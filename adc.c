/**
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */

#include "adc.h"
#include <msp430.h>

unsigned int adc[6] = {0};	// This will hold the adc values

char ConfigureADC(ADC_Channel ch)
{
	unsigned int ret;
    switch(ch)
    {
    case A0:
			ADC10CTL1 = INCH_0 + CONSEQ_1; // CH0, sequence conversation once
			ADC10CTL0 = ADC10SHT_2 + MSC + ADC10ON + ADC10IE;
			ADC10DTC1 = 0x01;              // 1 conversions
			ADC10AE0 |= 0x01;              // Enable analog input on P1.0
			ret = 1;
        	break;
    case A1:
			ADC10CTL1 = INCH_1 + CONSEQ_1; // CH1, sequence conversation once
			ADC10CTL0 = ADC10SHT_2 + MSC + ADC10ON + ADC10IE;
			ADC10DTC1 = 0x02;              // 2 conversions
			ADC10AE0 |= 0x02;              // Enable analog input on P1.1
			ret = 1;
        break;
    case A2:
			ADC10CTL1 = INCH_2 + CONSEQ_1; // CH0, sequence conversation once
			ADC10CTL0 = ADC10SHT_2 + MSC + ADC10ON + ADC10IE;
			ADC10DTC1 = 0x03;              // 3 conversions
			ADC10AE0 |= 0x03;              // Enable analog input on P1.2
			ret = 1;
        break;
    case A3:
			ADC10CTL1 = INCH_3 + CONSEQ_1; // CH0, sequence conversation once
			ADC10CTL0 = ADC10SHT_2 + MSC + ADC10ON + ADC10IE;
			ADC10DTC1 = 0x04;              // 4 conversions
			ADC10AE0 |= 0x04;              // Enable analog input on P1.3
			ret = 1;
        break;
    case A4:
			ADC10CTL1 = INCH_4 + CONSEQ_1; // CH4, sequence conversation once
			ADC10CTL0 = ADC10SHT_2 + MSC + ADC10ON + ADC10IE;
			ADC10DTC1 = 0x05;              // 5 conversions
			ADC10AE0 |= 0x05;              // Enable analog input on P1.4
			ret = 1;
        break;
    case A5:
			ADC10CTL1 = INCH_5 + CONSEQ_1; // CH5, sequence conversation once
			ADC10CTL0 = ADC10SHT_2 + MSC + ADC10ON + ADC10IE;
			ADC10DTC1 = 0x06;              // 6 conversions
			ADC10AE0 |= 0x06;              // Enable analog input on P1.5
			ret = 1;
        break;

    default:
        //#error  Selected channel is not valid.
        ret = 0;
       break;

    }

	return ret;
}

unsigned int GetADCValue(ADC_Channel ch)
{
	unsigned int result;
	
	ADC10CTL0 &= ~ENC; 				// Disable conversation
	while (ADC10CTL1 & BUSY);   	// Wait if ADC10 core is active
	ADC10SA = (unsigned int)adc;	/* Start address of DTC, write is required to 
												initiate DTC transfers. */
	ADC10CTL0 |= ENC + ADC10SC;   // Sampling and conversion start
	

    switch(ch)
    {
    case A0:
			result = adc[0];
        	break;
    case A1:
			result = adc[1];
        break;
    case A2:
			result = adc[2];
        break;
    case A3:
			result = adc[3];
        break;
    case A4:
			result = adc[4];
        break;
    case A5:
			result = adc[5];
        break;

    default:
        result = 0;
       break;

    }

	return result;
}
