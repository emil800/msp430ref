#include <msp430.h>
#include "pwm.h"
#define GLED BIT6                        // Green LED -> P1.6
#define RLED BIT0

// Global variables
unsigned int ADC_value=0;
 int adc[30] = {0}; //Sets up an array of 10 integers and zero's the values
 int avg_adc = 0;
int duty = 0;
// Function prototypes
void ConfigureAdc(void);
void adc_Sam10();
int main(void)
{


    char a,b;
    PWMPorts port1 = P2_2;
    PWMPorts port2 = P2_5;
    PWMPorts port3 = P1_6;
    unsigned char d1 = 1;
    unsigned char d2 = 100;
    unsigned char d3 = 200;
    unsigned int f = 255;


    WDTCTL = WDTPW + WDTHOLD;           // Stop watchdog timer
    /* Configure the clock module - MCLK = 1MHz */
    DCOCTL = 0;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;



    a = SetPWMOut(port1,d1,f);
    a = SetPWMOut(port2,d2,f);
    a = SetPWMOut(port3,d3,f);


    P1SEL |= BIT3;                  // ADC input pin P1.3
    ConfigureAdc();                 // ADC set-up function call

    while(1)
    {
        __delay_cycles(1000);               // Wait for ADC Ref to settle
        //ADC10CTL0 |= ENC + ADC10SC;         // Sampling and conversion start
        //__bis_SR_register(CPUOFF + GIE);    // Low Power Mode 0 with interrupts enabled
        //ADC_value = ADC10MEM;               // Assigns the value held in ADC10MEM to the integer called ADC_value
        adc_Sam10();        // Function call for adc_samp
        // Add all the sampled data and divide by 10 to find average
        avg_adc = ((adc[0]+adc[1]+adc[2]+adc[3]+adc[4]+adc[5]+adc[6]+adc[7]+adc[8]+adc[9]+adc[10]+adc[11]+adc[12]+adc[13]+adc[14]+adc[15]+adc[16]+adc[17]+adc[18]+adc[19]+adc[20]+adc[21]+adc[22]+adc[23]+adc[24]+adc[25]+adc[26]+adc[27]+adc[28]+adc[29]) / 30);
        duty = duty + (avg_adc-duty)/64;
        if (duty > 0){
            d3 = (unsigned char)(duty/4);

        }else{d3 = 0;}

        d2 =d2+1;
        b= SetPWMOut(port3,d3,f);
        a = SetPWMOut(port2,d2,f);

    }






    //TA1CTL = TASSEL_2 + MC_1; //TASSEL_2 selects SMCLK as the clock source, and MC_1 tells it to count up to the value in TA0CCR0.
    //__bis_SR_register(LPM3_bits); // Enter LPM3

   __bis_SR_register(LPM0_bits + GIE); // Goto LPM3 (Only ACLK active), Enable CPU Interrupt
}
// ADC sample conversion function
void adc_Sam10()
{
    ADC10CTL0 &= ~ENC;              // Disable Conversion
    while (ADC10CTL1 & BUSY);       // Wait if ADC10 busy
    ADC10SA = (int)adc;             // Transfers data to next array (DTC auto increments address)
    ADC10CTL0 |= ENC + ADC10SC;     // Enable Conversion and conversion start
    //__bis_SR_register(CPUOFF + GIE);// Low Power Mode 0, ADC10_ISR
}
// Function containing ADC set-up
void ConfigureAdc(void)
{
    ADC10CTL1 = CONSEQ_2 + INCH_3;                      // Repeat single channel, A0
    ADC10CTL0 = SREF_0 +ADC10SHT_3 + MSC + ADC10ON + ADC10IE;   // Sample & Hold Time + ADC10 ON + Interrupt Enable
    ADC10DTC1 = 30;                                   // 10 conversions

   // ADC10CTL1 = INCH_3 + ADC10DIV_3 ;         // Channel 3, ADC10CLK/3
    //ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON + ADC10IE;  // Vcc & Vss as reference, Sample and hold for 64 Clock cycles, ADC on, ADC interrupt enable
    //ADC10AE0 |= BIT3;                         // ADC input enable P1.3
}
/*
void __attribute__((interrupt(TIMER0_A0_VECTOR))) TimerA0_ISR(void)
{
    TACCR2 = 1000;                      // Set Timer Timeout Value
    TACCR1 = 3000;                      // Set Timer Timeout Value
    TA1CCR2 += 100;
    if (TA1CCR2 >=1000){
        TA1CCR2 = 0;
    }
}
void __attribute__((interrupt(TIMER0_A1_VECTOR))) TimerA1_ISR(void)
{
    switch (TAIV)
    {
        case 2: P1OUT ^= RLED;
            break;
        case 4: P1OUT ^= GLED;                       // Toggle LED
            break;
        case 10: P1OUT ^= RLED;                       // Toggle LED
            break;

    }

}
*/
