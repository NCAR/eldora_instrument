/*
 *	$Id$
 *
 *	Module:	TIMON	 
 *	Original Author: Shawn B. Copeland
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description:
 *        
 *  Starts the Timing Module
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

  
#include "/ELDORA/vx/mhen/include/ELDRP7.h"

timon()
{
   unsigned short *timon;
 
   timon = (unsigned short *)(TIMBASE + TIMON);
   *timon = 0x0000;
}
