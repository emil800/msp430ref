/*
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
 *
 *
 *
    P1.1 - TA0.0 - TA0CCR0
    P1.2 - TA0.1 - TA0CCR1
    P1.5 - TA0.0 - TA0CCR0
    P1.6 - TA0.1 - TA0CCR1
    P2.0 - TA1.0 - TA1CCR0
    P2.1 - TA1.1 - TA1CCR1
    P2.2 - TA1.1 - TA1CCR1
    P2.3 - TA1.0 - TA1CCR0
    P2.4 - TA1.2 - TA1CCR2
    P2.5 - TA1.2 - TA1CCR2
    P2,6 - TA0.1 - TA0CCR1
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
 *
 *
 * */

#ifndef PWM_H_
#define PWM_H_
/*PWM enabled pins on msp430g2*/
typedef enum Port_Enum{
    P1_2,/*TA0.1*/
    P2_1,/*TA1.1*/
    P2_2,/*TA1.1*/
    P2_4,/*TA1.2*/
    P2_5,/*TA1.2*/
    P1_6,/*TA0.1*/
    P2_6,/*TA0.1*/
} PWMPorts;



/*Set PWM output on specified port*/
char SetPWMOut(PWMPorts port, unsigned char duty_Q8, unsigned char perid);
#endif
