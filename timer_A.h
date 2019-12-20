/**
 *
 *
 *	MCx 	Mode 				Description
 *	00 		Stop 				The timer is halted.
 *	01 		Up 					The timer repeatedly counts from zero to the value of TACCR0.
 *	10 		Continuous 			The timer repeatedly counts from zero to 0FFFFh.
 *	11 		Up/down 			The timer repeatedly counts from zero up to the value of TACCR0 and back down to zero.
 *
 * Up Mode
 * TACCR0 -> Period
 *	The TACCR0 CCIFG interrupt flag is set when the timer counts to the TACCR0 value. The TAIFG
 *	interrupt flag is set when the timer counts from TACCR0 to zero.
 *
 * Continuous Mode
 *	In the continuous mode, the timer repeatedly counts up to 0FFFFh and restarts from zero
 *
 * Up/Down Mode
 *	The timer repeatedly counts up to the value of compare register TACCR0 and
 *	back down to zero
 *
 *	OUTMODx 	Mode 				Description
 *	000 		Output 				The output signal OUTx is defined by the OUTx bit. The OUTx signal updates immediately when OUTx is updated.
 *	001 		Set 				The output is set when the timer counts to the TACCRx value. It remains set until a reset of the timer, or until another output mode is selected and affects the output.
 *	010 		Toggle/Reset 		The output is toggled when the timer counts to the TACCRx value. It is reset when the timer counts to the TACCR0 value.
 *	011 		Set/Reset 			The output is set when the timer counts to the TACCRx value. It is reset when the timer counts to the TACCR0 value.
 *	100 		Toggle 				The output is toggled when the timer counts to the TACCRx value. The output period is double the timer period.
 *	101 		Reset 				The output is reset when the timer counts to the TACCRx value. It remains reset until another output mode is selected and affects the output.
 *	110 		Toggle/Set 			The output is toggled when the timer counts to the TACCRx value. It is set when the timer counts to the TACCR0 value.
 *	111 		Reset/Set 			The output is reset when the timer counts to the TACCRx value. It is set when the timer counts to the TACCR0 value.
 *
 *
 *
 *
 */

/**** include guard ***********************************************************/
#ifndef TIMER_A_HINC
#define TIMER_A_HINC

/**** includes ****************************************************************/



/**** public and callback definitions *****************************************/




/**** type definitions and macros *********************************************/


/**** start of extern C declaration section for C++ compatibility *************/

/**** variable declarations and definitions ***********************************/


/**** functions declarations **************************************************/


/**** inline functions ********************************************************/


/**** end of extern C declaration section for C++ compatibility ***************/

#endif /* TIMER_A_HINC */
/**** end of file *************************************************************/
