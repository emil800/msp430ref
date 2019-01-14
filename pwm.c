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

#include "pwm.h"
#include <msp430.h>



char SetPWMOut(PWMPorts port, unsigned char duty_Q8, unsigned char period)
{
    unsigned int highTime;
    unsigned int periodX8 = period * 8;
    highTime = (unsigned int)(8 * duty_Q8);


    switch(port)
    {
    case P1_2:
        P1DIR |= BIT2;/*Select direction*/
        P1SEL |= BIT2;
        TA0CCR0 = periodX8;/*Period*/
        TA0CCR1 = highTime; /*On time*/
        TA0CCTL1 = OUTMOD_7;
        TA0CTL = TASSEL_2 + MC_1; // SMCLK, up mode
        break;
    case P2_1:
        P2DIR |= BIT1;/*Select direction*/
        P2SEL |= BIT1;
        TA1CCR0 = periodX8;/*Period*/
        TA1CCR1 = highTime; /*On time*/
        TA1CCTL1 = OUTMOD_7;
        TA1CTL = TASSEL_2 + MC_1; // SMCLK, up mode
        break;
    case P2_2:
        P2DIR |= BIT2;/*Select direction*/
        P2SEL |= BIT2;
        TA1CCR0 = periodX8;/*Period*/
        TA1CCR1 = highTime; /*On time*/
        TA1CCTL1 = OUTMOD_7;
        TA1CTL = TASSEL_2 + MC_1; // SMCLK, up mode
        break;
    case P2_4:
        P2DIR |= BIT4;/*Select direction*/
        P2SEL |= BIT4;
        TA1CCR0 = periodX8;/*Period*/
        TA1CCR2 = highTime; /*On time*/
        TA1CCTL2 = OUTMOD_7;
        TA1CTL = TASSEL_2 + MC_1; // SMCLK, up mode
        break;
    case P2_5:
        P2DIR |= BIT5;/*Select direction*/
        P2SEL |= BIT5;
        TA1CCR0 = periodX8;/*Period*/
        TA1CCR2 = highTime; /*On time*/
        TA1CCTL2 = OUTMOD_7;
        TA1CTL = TASSEL_2 + MC_1; // SMCLK, up mode
        break;
    case P1_6:
        P1DIR |= BIT6;/*Select direction*/
        P1SEL |= BIT6;
        TA0CCR0 = periodX8;/*Period*/
        TA0CCR1 = highTime; /*On time*/
        TA0CCTL1 = OUTMOD_7;
        TA0CTL = TASSEL_2 + MC_1; // SMCLK, up mode
        break;
    default:
        //#error  Selected port is not a valid PWM port.
       break;



return 0;


    }



}
