/*
 *	$Id$
 *
 *	Module:	pwr1.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/08/14  21:43:32  reif
 * Initial revision
 *
 *
 * description: The purpose of this module is to set up the MZ7500 GPIB
 *              board as controller with DMA and interrupts in order to
 *              send commands to and receive data from the two WAVETEK 8502A
 *              Peak Power Meters used for fore and aft peak power 
 *              measurements.  The program also converts the string data from 
 *              the 8502A to binary and writes it to tape.
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include <pwrIncl.h>

void norm_isr()
{
unsigned char *csr,*chr;

csr = (unsigned char *)(DMACHAN0 + DMAOFF0);
chr = (unsigned char *)(DMACHAN0 + DMAOFF5);
*csr=0xff;
*chr=0x00;
*chr=0x10;
return;
}