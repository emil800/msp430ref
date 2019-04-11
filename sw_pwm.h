/*
 * sw_pwm.h
 *
 *  Created on: Apr 10, 2019
 *      Author: t800
 */

#ifndef SW_PWM_H_
#define SW_PWM_H_
/*SW controlled PWM pins on Port2*/
typedef struct {
   char    pwm_0;
   char    pwm_1;
   char    pwm_2;
} pwm_pins;


void configure_SWPWM(pwm_pins port2);
/*Set PWM output on specified pin*/
char SetSWPWMOut(unsigned char duty_Q8_0, unsigned char duty_Q8_1, unsigned char duty_Q8_2);

#endif /* SW_PWM_H_ */
