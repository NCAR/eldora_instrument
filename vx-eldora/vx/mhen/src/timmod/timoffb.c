/*
 *	$Id$
 *
 *	Module:	TIMOFFB
 *	Original Author: Shawn B. Copeland
 *      Copyright National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description:
 *        
 *  Starts the Timing Module, for rev. B board
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

  
#include "ELDRP7.h"

timoffb()
{
   unsigned short *timonoff;
 
   timonoff = (unsigned short *)(TIMBASE + TIMONOFFB);
   *timonoff = 0x0000; /* value is 0x00 to accomodate rev. B board */
}

