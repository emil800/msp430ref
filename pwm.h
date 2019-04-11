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
} PWMPorts;



/*Set PWM output on specified port*/
char SetHWPWMOut(PWMPorts port, unsigned char duty_Q8, unsigned char perid);
#endif
