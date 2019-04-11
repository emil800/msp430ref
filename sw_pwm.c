/*
 * sw_pwm.c
 *
 *  Created on: Apr 10, 2019
 *      Author: t800
 *
 *      Cofigure : Select pins for pwm output.
 *      use defines to set/reset pin states
 *      only change CCR values by input duty cycle
 *      This sw pwm is only for port 2
 *
 */

#include "sw_pwm.h"
#include <msp430.h>

pwm_pins out; /*module global variable to hold selected pins*/
unsigned char sum_out;

void configure_SWPWM(pwm_pins port2)
{
    out = port2;
    sum_out = port2.pwm_0 + port2.pwm_1 + port2.pwm_2;
    P2DIR |= sum_out; /*configure pins as output*/
    P2OUT &= ~(sum_out); /*set outputs to low*/
    /*enable CCr0, CCr1 and CCR2 interrupts*/
    TA1CCTL0 = CCIE;
    TA1CCTL1 = CCIE;
    TA1CCTL2 = CCIE;
    /*Timer1_A3 configuration*/
    TA1CTL = TASSEL_2 + MC_2 + TAIE; //SMCLK, Cont up mode, int enabled
}
char SetSWPWMOut(unsigned char duty_Q8_0, unsigned char duty_Q8_1, unsigned char duty_Q8_2)
{


    TA1CCR0 = (0xFFFF) - (unsigned int)duty_Q8_0 * 0xFF;
    TA1CCR1 = (0xFFFF) - (unsigned int)duty_Q8_1 * 0xFF;
    TA1CCR2 = (0xFFFF) - (unsigned int)duty_Q8_2 * 0xFF;

    return 0;
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
    P2OUT |= out.pwm_0;
}

// Timer_A1 Interrupt Vector (TA0IV) handler
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
      case  2: P2OUT |= out.pwm_1;
          break;
      case  4: P2OUT |= out.pwm_2;
          break;
      case 10: P2OUT &= ~(sum_out);                // Timer1_A3 overflow
          break;
     }
}
