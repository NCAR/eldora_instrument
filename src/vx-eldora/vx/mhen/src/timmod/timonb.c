/*
 *	$Id$
 *
 *	Module:	TIMONB
 *	Original Author: Shawn B. Copeland
 *      Copyright National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/09/10  21:32:15  shawn
 * Initial revision
 *
 *
 * description:
 *        
 *  Starts the Timing Module, for rev. B board
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

  
#include "ELDRP7.h"

timonb()
{
   unsigned short *timonoff;
 
   timonoff = (unsigned short *)(TIMBASE + TIMONOFFB);
   *timonoff = 0x0001; /* changed value to 0x01 to accomodate rev. B board */
}

