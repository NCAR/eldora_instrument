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
 
   timoff = (unsigned short *)(TIMBASE + TIMOFF);
   *timoff = 0x0000;
}
