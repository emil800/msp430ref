/**

 */

/**** include guard ***********************************************************/
#ifndef GENERIC_HINC
#define GENERIC_HINC

/**** includes ****************************************************************/




/**** type definitions and macros *********************************************/

/**
 * \brief	32-bit Macro for hardware access
 * \details Returns the value of the given HW register address
 */
#define HWREG32(x)                                                              \
    (*((volatile uint32_t *)((uint16_t)x)))
/**
 * \brief	16-bit Macros for hardware access
 * \details Returns the value of the given HW register address
 */
#define HWREG16(x)                                                             \
    (*((volatile uint16_t *)((uint16_t)x)))
/**
 * \brief	8-bit Macros for hardware access
 * \details Returns the value of the given HW register address
 */
#define HWREG8(x)                                                             \
    (*((volatile uint8_t *)((uint16_t)x)))

/**
 * \brief	Sets the given bit/s on given register
 * \details reg uint16_t, bits uint16_t
 */
#define BitsOn(reg, bits) 			((uint16_t)(reg) |= (uint16_t)(bits))

/**
 * \brief	Resets the given bit/s on given register
 * \details reg uint16_t, bits uint16_t
 */
#define BitsOff(reg, bits) 			((uint16_t)(reg) &= (uint16_t)(~bits))

/**
 * \brief	Toggles the given bit/s on given register
 * \details reg uint16_t, bits uint16_t
 */
#define BitsToggle(reg, bits) 			((uint16_t)(reg) ^= (uint16_t)(bits))

/**
 * \brief	Read the given bit value of given register
 * \details reg uint16_t, bit uint16_t
 * \return	return uint16_t 0 or 1
 */
#define BitRead_16U(reg, bit) 			(uint16_t)((uint16_t)(reg) & (uint16_t)(bits))




/**
 * \brief	Offset to PxIN from base address
 * \details P1IN               = 0x0020 \n
 * 			P2IN               = 0x0028
 */
#define OFS_PxIN 						0x0000

/**
 * \brief	Offset to PxOUT from base address
 * \details P1IN               = 0x0020 \n
 *			P1OUT              = 0x0021 \n
 *			P2IN               = 0x0028 \n
 *			P2OUT              = 0x0029
 */
#define OFS_PxOUT 						0x0001

/**
 * \brief	Offset to PxDIR from base address
 * \details P1IN               = 0x0020 \n
 *			P1DIR              = 0x0022 \n
 *			P2IN               = 0x0028 \n
 *			P2DIR              = 0x002A
 */
#define OFS_PxDIR 						0x0002

/**
 * \brief	Offset to PxIFG from base address
 * \details P1IN               = 0x0020 \n
 *			P1IFG              = 0x0023 \n
 *			P2IN               = 0x0028 \n
 *			P2IFG              = 0x002B
 */
#define OFS_PxIFG 						0x0003

/**
 * \brief	Offset to PxIES from base address
 * \details P1IN               = 0x0020 \n
 *			P1IES              = 0x0025 \n
 *			P2IN               = 0x0028 \n
 *			P2IES              = 0x002C
 */
#define OFS_PxIES 						0x0004

/**
 * \brief	Offset to PxIE from base address
 * \details P1IN               = 0x0020 \n
 *			P1IE               = 0x0025 \n
 *			P2IN               = 0x0028 \n
 *			P2IE               = 0x002D
 */
#define OFS_PxIE 						0x0005

/**
 * \brief	Offset to PxSEL from base address
 * \details P1IN               = 0x0020 \n
 *			P1SEL              = 0x0026 \n
 *			P2IN               = 0x0028 \n
 *			P2SEL              = 0x002E
 */
#define OFS_PxSEL 						0x0006

/**
 * \brief	Offset to PxREN from base address
 * \details P1IN               = 0x0020 \n
 *			P1REN              = 0x0027 \n
 *			P2IN               = 0x0028 \n
 *			P2REN              = 0x002F
 */
#define OFS_PxREN 						0x0007

/**
 * \brief	Offset to PxSEL2 from base address
 * \details P1IN               = 0x0020 \n
 *			P1SEL2             = 0x0041
 */
#define OFS_P1SEL2 					0x0021

/**
 * \brief	Offset to PxSEL2 from base address
 * \details P2IN               = 0x0028 \n
 *			P2SEL2             = 0x0042
 */
#define OFS_P2SEL2 					0x001A






/**** variable declarations and definitions ***********************************/


/**** functions declarations **************************************************/


/**** inline functions ********************************************************/


#endif /* GENERIC_HINC */
/**** end of file *************************************************************/
