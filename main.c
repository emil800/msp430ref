#include <msp430.h>
#include <stdint.h>

#include "pwm.h"
#include "adc.h"
#include "flash.h"

#define SETTINGS_SEG    FLASH_INFOD
#define SETTINGS_MAGIC  0xA55Au

typedef struct {
    uint16_t magic;
    uint8_t  duty_1;
    uint8_t  duty_2;
    uint16_t pwm_period;
} settings_t;

static settings_t settings;

static void init(void);
static void load_or_init_settings(void);

int main(void)
{
    init();
    load_or_init_settings();

    PWM_Configure(P2_1, settings.pwm_period);
    PWM_Configure(P2_4, settings.pwm_period);
    PWM_Configure(P2_6, settings.pwm_period);

    PWM_SetDuty(P2_1, settings.duty_1);
    PWM_SetDuty(P2_4, settings.duty_2);

    while (1) {
        /* ADC10 is 10-bit; map to 8-bit duty. */
        uint8_t duty_3 = (uint8_t)(GetADCValue(A0) >> 2);
        PWM_SetDuty(P2_6, duty_3);
    }
}

static void init(void)
{
    WDTCTL = WDTPW | WDTHOLD;

    DCOCTL  = 0;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL  = CALDCO_1MHZ;

    ConfigureADC(A0);
    ConfigureADC(A1);
    ConfigureADC(A2);
    ConfigureADC(A3);

    Flash_Init();
}

static void load_or_init_settings(void)
{
    Flash_Read(SETTINGS_SEG, 0, &settings, sizeof settings);

    if (settings.magic != SETTINGS_MAGIC) {
        settings.magic      = SETTINGS_MAGIC;
        settings.duty_1     = 0xDD;
        settings.duty_2     = 0xEE;
        settings.pwm_period = 255;

        Flash_EraseSegment(SETTINGS_SEG);
        Flash_Write(SETTINGS_SEG, 0, &settings, sizeof settings);
    }
}
