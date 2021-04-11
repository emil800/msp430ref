/*
 *
 *
 *
 * PxSEL2 PxSEL 	Pin Function
 *	0 		0 		I/O function is selected.
 *	0 		1 		Primary peripheral module function is selected.
 *	1 		0 		Reserved. See device-specific data sheet.
 *	1 		1 		Secondary peripheral module function is selected.
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

/**** own include *************************************************************/
#define  GPIO_OWN
#include "gpio.h"

/**** extern includes *********************************************************/
#include "generic.h"
//TODO:Include msp430g2

/**** type definitions and macros *********************************************/
const uint16_t GPIO_PORT_TO_BASE[] = {
    0x0000,
	&P1IN, 	/*0x0020*/
	&P2IN	/*0x0028*/
};

/**** variable definitions ****************************************************/


/**** function declarations ***************************************************/


/**** inline functions ********************************************************/


/**** functions ***************************************************************/
void GPIO_setAsOutputPin(uint8_t selectedPort,
                         uint16_t selectedPins) {
    uint16_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];

    if(selectedPort == 1)
    {
    	HWREG16(baseAddress + OFS_P1SEL2) &= ~selectedPins;
    }
    if(selectedPort == 2)
    {
    	HWREG16(baseAddress + OFS_P2SEL2) &= ~selectedPins;
    }


    HWREG16(baseAddress + OFS_PxSEL) &= ~selectedPins;
    HWREG16(baseAddress + OFS_PxDIR) |= selectedPins;

    return;
}

void GPIO_setAsInputPin(uint8_t selectedPort,
                        uint16_t selectedPins) {
    uint16_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];

    if(selectedPort == 1)
    {
    	HWREG16(baseAddress + OFS_P1SEL2) &= ~selectedPins;
    }
    if(selectedPort == 2)
    {
    	HWREG16(baseAddress + OFS_P2SEL2) &= ~selectedPins;
    }

    HWREG16(baseAddress + OFS_PxSEL) &= ~selectedPins;
    HWREG16(baseAddress + OFS_PxDIR) &= ~selectedPins;
    HWREG16(baseAddress + OFS_PxREN) &= ~selectedPins;
}

void GPIO_setAsPeripheralModuleFunctionOutputPin(uint8_t selectedPort,
                                                 uint16_t selectedPins) {
    uint16_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];


    HWREG16(baseAddress + OFS_PxDIR) |= selectedPins;
    HWREG16(baseAddress + OFS_PxSEL) |= selectedPins;
}

void GPIO_setAsPeripheralModuleFunctionInputPin(uint8_t selectedPort,
                                                uint16_t selectedPins) {
    uint16_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];


    HWREG16(baseAddress + OFS_PxDIR) &= ~selectedPins;
    HWREG16(baseAddress + OFS_PxSEL) |= selectedPins;
}

/*TODO:
 *
 * Include function to set as secondary
 * PeripheralModule Function usinf PxSEL2
 * */

void GPIO_setAsInputPinWithPullDownResistor(uint8_t selectedPort,
                                            uint16_t selectedPins) {
    uint16_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];


    HWREG16(baseAddress + OFS_PxSEL) &= ~selectedPins;
    HWREG16(baseAddress + OFS_PxDIR) &= ~selectedPins;
    HWREG16(baseAddress + OFS_PxREN) |= selectedPins;
    HWREG16(baseAddress + OFS_PxOUT) &= ~selectedPins;
}

void GPIO_setAsInputPinWithPullUpResistor(uint8_t selectedPort,
                                          uint16_t selectedPins) {
    uint16_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];


    HWREG16(baseAddress + OFS_PxSEL) &= ~selectedPins;
    HWREG16(baseAddress + OFS_PxDIR) &= ~selectedPins;
    HWREG16(baseAddress + OFS_PxREN) |= selectedPins;
    HWREG16(baseAddress + OFS_PxOUT) |= selectedPins;
}

void GPIO_enableInterrupt(uint8_t selectedPort,
                          uint16_t selectedPins) {
    uint16_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];


    HWREG16(baseAddress + OFS_PxIE) |= selectedPins;
}

void GPIO_disableInterrupt(uint8_t selectedPort,
                           uint16_t selectedPins) {
    uint16_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];

    HWREG16(baseAddress + OFS_PxIE) &= ~selectedPins;
}

uint16_t GPIO_getInterruptStatus(uint8_t selectedPort,
                                 uint16_t selectedPins) {
    uint16_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];

    return (HWREG16(baseAddress + OFS_PxIFG) & selectedPins);

}

void GPIO_clearInterrupt(uint8_t selectedPort,
                         uint16_t selectedPins) {
    uint16_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];

    HWREG16(baseAddress + OFS_PxIFG) &= ~selectedPins;
}

void GPIO_selectInterruptEdge(uint8_t selectedPort,
                              uint16_t selectedPins,
                              uint8_t edgeSelect) {
    uint16_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];


    if(GPIO_LOW_TO_HIGH_TRANSITION == edgeSelect)
    {
        HWREG16(baseAddress + OFS_PxIES) &= ~selectedPins;
    }
    if (GPIO_HIGH_TO_LOW_TRANSITION == edgeSelect)
    {
        HWREG16(baseAddress + OFS_PxIES) |= selectedPins;
    }
}


/**** end of file *************************************************************/
