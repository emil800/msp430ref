/*
 * MSP430G2553 information-memory flash driver.
 *
 *  Segment layout (see msp430g2553.ld / user guide slau144j):
 *    INFOA  0x10C0 - 0x10FF   holds DCO calibration constants, do NOT erase
 *    INFOB  0x1080 - 0x10BF   free
 *    INFOC  0x1040 - 0x107F   free
 *    INFOD  0x1000 - 0x103F   free
 *
 *  Each segment is 64 bytes. Flash can only be cleared by whole-segment erase;
 *  individual bits can be cleared (1 -> 0) without erase, but cannot be set
 *  back without erasing the whole segment first.
 *
 *  Flash_Init() must run once after the DCO is calibrated (so SMCLK is at a
 *  known rate); the driver sets the flash timing generator to ~333 kHz from
 *  a 1 MHz SMCLK, which is inside the 257..476 kHz window required by the
 *  flash controller.
 */

#ifndef FLASH_H_
#define FLASH_H_

#include <stdint.h>

typedef enum {
    FLASH_INFOD = 0x1000,
    FLASH_INFOC = 0x1040,
    FLASH_INFOB = 0x1080,
} FlashSegment;

#define FLASH_SEG_SIZE 64u

void Flash_Init(void);

char Flash_EraseSegment(FlashSegment seg);

char Flash_Write(FlashSegment seg,
                 uint8_t      offset,
                 const void  *src,
                 uint8_t      len);

void Flash_Read(FlashSegment seg,
                uint8_t      offset,
                void        *dst,
                uint8_t      len);

#endif /* FLASH_H_ */
