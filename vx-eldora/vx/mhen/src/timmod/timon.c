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
 * Revision 1.1  1991/12/04  19:06:43  eric
 * Initial revision
 *
 *
 * description:
 *        
 *  Starts the Timing Module
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

  
#include "ELDRP7.h"

timon()
{
   unsigned short *timon;
 
   timon = (unsigned short *)(TIMBASE + TIMON);
   *timon = 0x0000;
}
