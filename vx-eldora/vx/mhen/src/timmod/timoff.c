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
 *
 * description:
 *
 *  Stops the Timing Module 
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";


#include "/ELDORA/vx/mhen/include/ELDRP7.h"

timoff()
{
   unsigned short *timoff;
 
   timoff = (unsigned short *)(TIMBASE + TIMOFF);
   *timoff = 0x0000;
}
