/*
 *
 *	$Id$
 *
 *	Module:	motor.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description:
 *             
 *             
 *              
 *              
 *             
 *             
 *             
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include <cntrlIncl.h>


void init_motor()
{
unsigned char *ltch;

ltch=(unsigned char *)CONTROL_LATCH;
*ltch=0x06; /* Latch 1100 reset */
taskDelay(1);
*ltch=0x0E; /* Release 1100 reset */

return;
}
