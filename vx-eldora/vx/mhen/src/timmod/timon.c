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
 * Revision 1.2  1992/09/10  20:40:43  eric
 * *** empty log message ***
 *
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
   *timon = 0x0001;  /* changed value to 0x01 to accomodate rev. B board */
}
