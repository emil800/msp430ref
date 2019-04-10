//******************************************************************************
//  MSP430G2xx3 Demo - Timer_A, Toggle P1.0-2, Cont. Mode ISR, DCO SMCLK
//
//  Description: Use Timer_A CCRx units and overflow to generate four
//  independent timing intervals. For demonstration, CCR0 and CCR1 output
//  units are optionally selected with port pins P1.1 and P1.2 in toggle
//  mode. As such, these pins will toggle when respective CCRx registers match
//  the TAR counter. Interrupts are also enabled with all CCRx units,
//  software loads offset to next interval only - as long as the interval offset
//  is aded to CCRx, toggle rate is generated in hardware. Timer_A overflow ISR
//  is used to toggle P1.0 with software. Proper use of the TA0IV interrupt
//  vector generator is demonstrated.
//  ACLK = n/a, MCLK = SMCLK = TACLK = default DCO ~1MHz
//  As coded and assuming ~1MHz DCO, toggle rates are:
//  P1.1 = CCR0 ~ 1MHz/(2*200) ~2500Hz
//  P1.2 = CCR1 ~ 1MHz/(2*1000) ~500Hz
//  P1.0 = overflow ~ 1MHz/(2*65536) ~8Hz
//
//               MSP430G2xx3
//            -----------------
//        /|\|              XIN|-
//         | |                 |
//         --|RST          XOUT|-
//           |                 |
//           |         P1.1/TA0|--> CCR0
//           |         P1.2/TA1|--> CCR1
//           |             P1.0|--> Overflow/software
//
//  D. Dang
//  Texas Instruments Inc.
//  December 2010
//   Built with CCS Version 4.2.0 and IAR Embedded Workbench Version: 5.10
//******************************************************************************
#ifdef SOFTWARE_PWM_ON_PORT2_USING_TIMER1
#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  /* Configure the clock module - MCLK = 1MHz */
      DCOCTL = 0;
      BCSCTL1 = CALBC1_8MHZ;
      DCOCTL = CALDCO_8MHZ;


  //P2SEL |= BIT1 + BIT3 + BIT4;//0x14;                            // P2.2 - P2.3 - P2.4 option select 0x00010100
  P2DIR |= BIT0 + BIT1 + BIT3 + BIT4;//0x15;                            // P2.0, P2.2, P2.3, P2.4 outputs
  TA1CCTL0 = OUTMOD_2 + CCIE;                  // CCR0 toggle, interrupt enabled
  TA1CCTL1 = OUTMOD_2 + CCIE;                  // CCR1 toggle, interrupt enabled
  TA1CCTL2 = OUTMOD_2 + CCIE;                  // CCR1 toggle, interrupt enabled
  //P2OUT &= 0x00;
  TA1CCR0 = 500;
  TA1CCR1 = 100;
  TA1CCR2 = 30000;
  TA1CTL = TASSEL_2 + MC_2 + TAIE; //SMCLK, Contmode, int enabled
  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt

}

// Timer A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer_A0 (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_A0_VECTOR))) Timer1_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    //TA1CCR0 += 100;                              // Add Offset to CCR0
    P2OUT |= BIT1;
}

// Timer_A2 Interrupt Vector (TA0IV) handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer_A1(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_A1_VECTOR))) Timer1_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch( TA1IV )
  {
  case  2: P2OUT |= BIT3;
                               // Add Offset to CCR1
           break;
  case  4: P2OUT |= BIT4;

          break;
  case 10: //P2OUT ^= BIT0;                   // Timer_A3 overflow
           TA1CCR1 += 200;
           TA1CCR2 += 50;
           TA1CCR0 += 100;
           P2OUT &= 0x00;


           break;
 }
}
#endif
#ifdef ADC_MULTIPLE_CHANNEL

#include <msp430.h>
#define GLED BIT6                        // Green LED -> P1.6
#define RLED BIT0

// Global variables
unsigned int ADC_value=0;
 int adc[5] = {0}; //Sets up an array of 5 integers and zero's the values
 int avg_adc = 0;

// Function prototypes
void ConfigureAdc(void);
void adc_Sam10();
int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;           // Stop watchdog timer
    /* Configure the clock module - MCLK = 1MHz */
    DCOCTL = 0;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    P1IN |= (BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5);                  // ADC input pins

    ConfigureAdc();                 // ADC set-up function call

    while(1)
    {
        __delay_cycles(1000);               // Wait for ADC Ref to settle
        //ADC10CTL0 |= ENC + ADC10SC;         // Sampling and conversion start
        //__bis_SR_register(CPUOFF + GIE);    // Low Power Mode 0 with interrupts enabled
        //ADC_value = ADC10MEM;               // Assigns the value held in ADC10MEM to the integer called ADC_value
        adc_Sam10();        // Function call for adc_samp
        avg_adc = (adc[0] + adc[1] + adc[2] + adc[3] + adc[4])/5;
        P2DIR = 0x00;
        if (avg_adc > 100){
            P2DIR |= BIT0;
        }
        if (avg_adc > 250){
            P2DIR |= BIT1;
        }
        if (avg_adc > 400){
            P2DIR |= BIT2;
        }
        if (avg_adc > 550){
            P2DIR |= BIT3;
        }
        if (avg_adc > 700){
            P2DIR |= BIT4;
        }
        if (avg_adc > 750){
            P2DIR |= BIT5;
        }
    }


   __bis_SR_register(LPM0_bits + GIE); // Goto LPM0 , Enable CPU Interrupt
}

// ADC sample conversion function
void adc_Sam10()
{
    ADC10CTL0 &= ~ENC;              // Disable Conversion
    while (ADC10CTL1 & BUSY);       // Wait if ADC10 busy
    ADC10SA = (int)adc;             // Transfers data to next array (DTC auto increments address)
    ADC10CTL0 |= ENC + ADC10SC;     // Enable Conversion and conversion start

}
// Function containing ADC set-up
void ConfigureAdc(void)
{
    ADC10CTL1 = CONSEQ_3 + INCH_5;                      // Repeat single channel, A0
    ADC10CTL0 = SREF_0 + ADC10SHT_3 + MSC + ADC10ON + ADC10IE;   // Sample & Hold Time + ADC10 ON + Interrupt Enable
    ADC10DTC1 = 5;                                   // 5 conversions

    // ADC10CTL1 = INCH_3 + ADC10DIV_3 ;         // Channel 3, ADC10CLK/3
    //ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON + ADC10IE;  // Vcc & Vss as reference, Sample and hold for 64 Clock cycles, ADC on, ADC interrupt enable
    //ADC10AE0 |= BIT3;                         // ADC input enable P1.3
}
#endif


