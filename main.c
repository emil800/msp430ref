#include <msp430.h>
#include "pwm.h"
#include "adc.h"
// Global variables
char duty = 0;
// Function prototypes
void init(void);
void test_HWPWM(void);

int main(void)
{
	ADC_Channel channel0 = A0;
	init();
   

    while(1)
    {

		duty = (char)(GetADCValue(channel0) >> 2);
		test_HWPWM();

    }

   __bis_SR_register(LPM0_bits + GIE); /* Goto LPM3 (CPU, MCLK are disabled, 
   													SMCLK, ACLK are active), Enable CPU Interrupt */
}

void init(void)
{

    WDTCTL = WDTPW + WDTHOLD;           // Stop watchdog timer
    
    /* Configure the clock module - MCLK = 1MHz */
    DCOCTL = 0;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
    
    //Configure CH0 
    ADC_Channel channel0 = A0;
	 ConfigureADC(channel0);

}

void test_HWPWM(void)
{

    char a,b;
    PWMPorts port1 = P2_1;
    PWMPorts port2 = P2_4;
    PWMPorts port3 = P2_6;
    
    unsigned char d1 = 10;
    unsigned char d2 = 50;
    unsigned char d3 = duty;
    unsigned int  f 	= 255;


    a = SetPWMOut(port1,d1,f);
    a = SetPWMOut(port2,d2,f);
    a = SetPWMOut(port3,d3,f);

}


