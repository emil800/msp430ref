/**
 * ADC10, CONSEQ_1 (single sequence of channels, A_highest..A0).
 *
 * Design notes:
 *  - ConfigureADC() can be called multiple times; it accumulates the set of
 *    requested channels and reprograms the sequencer to span A_highest..A0.
 *  - ADC10 CONSEQ_1 converts from INCHx down to A0 and the DTC writes the
 *    samples into adc_buf[] in the order they are produced. So the first
 *    sample in the buffer is for the highest channel; GetADCValue() reverses
 *    that.
 *  - GetADCValue() triggers a fresh sequence and waits for completion, so it
 *    always returns a current sample rather than a stale one.
 */

#include "adc.h"
#include <msp430.h>
#include <stdint.h>

#define ADC_MAX_CHANNELS 6u

static volatile uint16_t adc_buf[ADC_MAX_CHANNELS];
static uint8_t           cfg_mask;
static uint8_t           highest_ch;

static void recompute_highest(void)
{
    highest_ch = 0;
    for (int8_t i = ADC_MAX_CHANNELS - 1; i >= 0; i--) {
        if (cfg_mask & (uint8_t)(1u << i)) {
            highest_ch = (uint8_t)i;
            break;
        }
    }
}

char ConfigureADC(ADC_Channel ch)
{
    if ((unsigned)ch >= ADC_MAX_CHANNELS) {
        return 0;
    }

    const uint8_t bit = (uint8_t)(1u << (uint8_t)ch);

    P1REN |=  bit;
    P1OUT &= (uint8_t)~bit;
    ADC10AE0 |= bit;

    cfg_mask |= bit;
    recompute_highest();

    ADC10CTL0 &= ~ENC;
    while (ADC10CTL1 & BUSY) { }

    ADC10CTL1 = (uint16_t)((uint16_t)highest_ch << 12) | CONSEQ_1;
    ADC10CTL0 = ADC10SHT_2 | MSC | ADC10ON;
    ADC10DTC1 = (uint8_t)(highest_ch + 1u);

    return 1;
}

unsigned int GetADCValue(ADC_Channel ch)
{
    if ((unsigned)ch >= ADC_MAX_CHANNELS) {
        return 0;
    }
    if (!(cfg_mask & (uint8_t)(1u << (uint8_t)ch))) {
        return 0;
    }

    ADC10CTL0 &= ~ENC;
    while (ADC10CTL1 & BUSY) { }

    ADC10SA    = (uint16_t)adc_buf;
    ADC10CTL0 |= ENC | ADC10SC;

    while (ADC10CTL1 & BUSY) { }

    /* CONSEQ_1 produces A_highest first, A0 last; buffer index is inverted. */
    return adc_buf[highest_ch - (uint8_t)ch];
}
