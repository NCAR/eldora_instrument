/*
 *	$Id$
 *
 *	Module:	TIMOFF	 
 *	Original Author: Shawn B. Copelane 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1992/09/10  20:40:58  eric
 * *** empty log message ***
 *
 * Revision 1.1  1991/12/04  19:06:51  eric
 * Initial revision
 *
 *
 * description:
 *
 *  Stops the Timing Module 
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";


#include "ELDRP7.h"

timoff()
{
   unsigned short *timoff;
 /* map Old Timing Module in VME Extended Space */ 
   timoff = (unsigned short *)(TIMBASE - STD_BASE + EXTD_BASE + TIMOFF);
   *timoff = 0x0000;
}
