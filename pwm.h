/*
 * Hardware PWM on MSP430G2553 Timer_A outputs.
 *
 *  Pin      Timer / CCR
 *  P1.2     TA0.1 (TA0CCR1)
 *  P1.6     TA0.1 (TA0CCR1)     shared with P1.2
 *  P2.1     TA1.1 (TA1CCR1)
 *  P2.2     TA1.1 (TA1CCR1)     shared with P2.1
 *  P2.4     TA1.2 (TA1CCR2)
 *  P2.5     TA1.2 (TA1CCR2)     shared with P2.4
 *  P2.6     TA0.1 (TA0CCR1)     shared with P1.2 / P1.6
 *
 *  Ports that share a CCR register cannot carry independent duty cycles.
 *
 *  TA0.1 — one of P1_2 / P1_6 / P2_6
 *  TA1.1 — one of P2_1 / P2_2
 *  TA1.2 — one of P2_4 / P2_5
 */

#ifndef PWM_H_
#define PWM_H_

#include <stdint.h>

typedef enum Port_Enum {
    P1_2,
    P2_1,
    P2_2,
    P2_4,
    P2_5,
    P1_6,
    P2_6,
    PWM_PORT_COUNT
} PWMPorts;

/* Configure pin direction, function-select, and Timer_A once.
 * period sets CCR0 via period * 8 timer ticks. */
char PWM_Configure(PWMPorts port, uint16_t period);

/* Update duty only. Must be preceded by PWM_Configure(port, ...).
 * duty_Q8 maps 0..0xFF to 0..period. */
char PWM_SetDuty(PWMPorts port, uint8_t duty_Q8);

/* Backward-compatible one-shot: Configure + SetDuty. */
char SetPWMOut(PWMPorts port, uint8_t duty_Q8, uint16_t period);

#endif /* PWM_H_ */
