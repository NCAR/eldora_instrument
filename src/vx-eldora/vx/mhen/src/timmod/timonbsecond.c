/*
 *	$Id$
 *
 *	Module:	TIMONBSECOND
 *	Original Author: Shawn B. Copeland
 *      Copyright National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1993/07/22  17:31:28  eric
 * Initial revision
 *
 * Revision 1.1  1992/09/10  21:32:15  shawn
 * Initial revision
 *
 *
 * description:
 *        
 *  Starts the Timing Module, for rev. B board
 *  NOTE:  THIS VERSION USES BASE ADDRESS FOR SECOND TIMING MODULE IN A
 *         CHASSIS!  IT IS ONLY FOR TESTING THE BOARD-TO-BOARD SYNC FUNCTION.
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

  
#include "ELDRP7.h"

timonbsecond()
{
   unsigned short *timonoff;
 
   timonoff = (unsigned short *)(TIMBASE2 + TIMONOFFB);
   *timonoff = 0x0001; /* changed value to 0x01 to accomodate rev. B board */
}

