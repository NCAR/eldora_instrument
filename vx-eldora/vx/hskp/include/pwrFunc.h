/*
 *	$Id$
 *
 *	Module:	pwrFunc.h	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description: The purpose of this module is to set up the MZ7500 GPIB
 *              board as controller with DMA and interrupts in order to
 *              send commands to and receive data from the two WAVETEK 8502A
 *              Peak Power Meters used for fore and aft peak power 
 *              measurements.  The program also converts the string data from 
 *              the 8502A to binary and writes it to tape.
 */

#ifndef INCpwrFunch
#define INCpwrFunch

/********************  FUNCTION PROTOTYPES *************************/

float pwr_menu(unsigned long);
void norm_isr(void);
void err_isr(void);

#endif /* INC */
