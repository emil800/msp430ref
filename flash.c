/**
 * Flash controller sequences for the MSP430G2553.
 *
 * Erase:
 *   FCTL3 = FWKEY;              clear LOCK (LOCKA stays as-is, it is toggle-write)
 *   FCTL1 = FWKEY | ERASE;      arm segment erase
 *   *any_addr_in_segment = 0;   dummy write triggers the erase
 *   wait BUSY -> 0
 *   FCTL1 = FWKEY;              clear ERASE
 *   FCTL3 = FWKEY | LOCK;       relock
 *
 * Write (byte-at-a-time):
 *   FCTL3 = FWKEY;
 *   FCTL1 = FWKEY | WRT;
 *   for each byte: *dst++ = *src++; wait BUSY -> 0;
 *   FCTL1 = FWKEY;
 *   FCTL3 = FWKEY | LOCK;
 *
 * Interrupts are disabled across the sequences: the CPU cannot fetch from
 * flash while an erase or program is in progress.
 */

#include "flash.h"
#include <msp430.h>
#include <stdint.h>
#include <string.h>

void Flash_Init(void)
{
    /* SMCLK = 1 MHz, divide by 3 -> ~333 kHz flash timing generator.
     * FCTL2.FN[5:0] holds (divisor - 1), so FN1 == 0b000010 == /3. */
    FCTL2 = FWKEY | FSSEL_2 | FN1;
}

char Flash_EraseSegment(FlashSegment seg)
{
    volatile uint8_t *p = (volatile uint8_t *)seg;

    __disable_interrupt();

    while (FCTL3 & BUSY) { }

    FCTL3 = FWKEY;
    FCTL1 = FWKEY | ERASE;
    *p    = 0;
    while (FCTL3 & BUSY) { }

    FCTL1 = FWKEY;
    FCTL3 = FWKEY | LOCK;

    __enable_interrupt();
    return 1;
}

char Flash_Write(FlashSegment seg, uint8_t offset, const void *src, uint8_t len)
{
    if ((uint16_t)offset + (uint16_t)len > FLASH_SEG_SIZE) {
        return 0;
    }

    volatile uint8_t *dst = (volatile uint8_t *)seg + offset;
    const uint8_t    *s   = (const uint8_t *)src;

    __disable_interrupt();

    while (FCTL3 & BUSY) { }

    FCTL3 = FWKEY;
    FCTL1 = FWKEY | WRT;

    for (uint8_t i = 0; i < len; i++) {
        dst[i] = s[i];
        while (FCTL3 & BUSY) { }
    }

    FCTL1 = FWKEY;
    FCTL3 = FWKEY | LOCK;

    __enable_interrupt();
    return 1;
}

void Flash_Read(FlashSegment seg, uint8_t offset, void *dst, uint8_t len)
{
    memcpy(dst, (const void *)((uintptr_t)seg + offset), len);
}
