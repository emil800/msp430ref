/*
 *
 *	CONSEQx 	Mode 											Operation
 *		00 	Single channel single-conversion 	A single channel is converted once.
 * 	01 	Sequence-of-channels 					A sequence of channels is converted once.
 * 	10 	Repeat single channel 					A single channel is converted repeatedly.
 * 	11 	Repeat sequence-of-channels 			A sequence of channels is converted repeatedly
 *
 *
 *
 * */

#ifndef ADC_H_
#define ADC_H_
/*PWM enabled pins on msp430g2*/
typedef enum ADC_CH{
    A0,/* CH0 */
    A1,/* CH1 */
    A2,/* CH2 */
    A3,/* CH3 */
    A4,/* CH4 */
    A5 /* CH5 */
} ADC_Channel;



/*Configure ADC */
char ConfigureADC(ADC_Channel ch);
unsigned int GetADCValue(ADC_Channel ch);
#endif
