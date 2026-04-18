/**
 * Hardware PWM driver using Timer_A0 and Timer_A1 in up mode with OUTMOD_7
 * (reset/set). Period is stored per port so SetDuty can rescale duty_Q8 into
 * timer ticks without re-touching the timer control registers.
 */

#include "pwm.h"
#include <msp430.h>
#include <stdint.h>

typedef struct {
    volatile unsigned int *ccrx;   /* CCRx registers are declared as unsigned int by msp430.h */
    uint16_t               period_x8;
    uint8_t                configured;
} pwm_runtime_t;

static pwm_runtime_t rt[PWM_PORT_COUNT];

static void configure_pins_and_timer(PWMPorts port, uint16_t period_x8)
{
    switch (port) {
    case P1_2:
        P1DIR |= BIT2;
        P1SEL |= BIT2;
        TA0CCR0  = period_x8;
        TA0CCTL1 = OUTMOD_7;
        TA0CTL   = TASSEL_2 | MC_1;
        rt[port].ccrx = &TA0CCR1;
        break;
    case P1_6:
        P1DIR |= BIT6;
        P1SEL |= BIT6;
        TA0CCR0  = period_x8;
        TA0CCTL1 = OUTMOD_7;
        TA0CTL   = TASSEL_2 | MC_1;
        rt[port].ccrx = &TA0CCR1;
        break;
    case P2_6:
        P2DIR  |=  BIT6;
        P2SEL  |=  BIT6;
        P2SEL2 &= (uint8_t)~BIT6;
        TA0CCR0  = period_x8;
        TA0CCTL1 = OUTMOD_7;
        TA0CTL   = TASSEL_2 | MC_1;
        rt[port].ccrx = &TA0CCR1;
        break;
    case P2_1:
        P2DIR |= BIT1;
        P2SEL |= BIT1;
        TA1CCR0  = period_x8;
        TA1CCTL1 = OUTMOD_7;
        TA1CTL   = TASSEL_2 | MC_1;
        rt[port].ccrx = &TA1CCR1;
        break;
    case P2_2:
        P2DIR |= BIT2;
        P2SEL |= BIT2;
        TA1CCR0  = period_x8;
        TA1CCTL1 = OUTMOD_7;
        TA1CTL   = TASSEL_2 | MC_1;
        rt[port].ccrx = &TA1CCR1;
        break;
    case P2_4:
        P2DIR |= BIT4;
        P2SEL |= BIT4;
        TA1CCR0  = period_x8;
        TA1CCTL2 = OUTMOD_7;
        TA1CTL   = TASSEL_2 | MC_1;
        rt[port].ccrx = &TA1CCR2;
        break;
    case P2_5:
        P2DIR |= BIT5;
        P2SEL |= BIT5;
        TA1CCR0  = period_x8;
        TA1CCTL2 = OUTMOD_7;
        TA1CTL   = TASSEL_2 | MC_1;
        rt[port].ccrx = &TA1CCR2;
        break;
    default:
        break;
    }
}

char PWM_Configure(PWMPorts port, uint16_t period)
{
    if ((unsigned)port >= PWM_PORT_COUNT) {
        return 0;
    }
    /* period * 8 must fit in a 16-bit timer counter. */
    if (period > (0xFFFFu / 8u)) {
        return 0;
    }

    const uint16_t period_x8 = (uint16_t)(period * 8u);

    rt[port].period_x8  = period_x8;
    rt[port].configured = 1;

    configure_pins_and_timer(port, period_x8);
    *rt[port].ccrx = 0;
    return 1;
}

char PWM_SetDuty(PWMPorts port, uint8_t duty_Q8)
{
    if ((unsigned)port >= PWM_PORT_COUNT) {
        return 0;
    }
    if (!rt[port].configured) {
        return 0;
    }

    uint16_t high;
    if (duty_Q8 == 0xFF) {
        high = rt[port].period_x8;
    } else {
        /* duty_Q8/256 * period_x8, cheap on MSP430. */
        high = (uint16_t)(((uint32_t)duty_Q8 * rt[port].period_x8) >> 8);
    }

    *rt[port].ccrx = high;
    return 1;
}

char SetPWMOut(PWMPorts port, uint8_t duty_Q8, uint16_t period)
{
    if (!PWM_Configure(port, period)) {
        return 0;
    }
    return PWM_SetDuty(port, duty_Q8);
}
