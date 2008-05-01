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
 * Revision 1.3  1993/05/12  16:18:52  eric
 * modified code to map OLD Timing Module to VME Extended
 * Address Space.
 *
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

#ifndef SPOL
#include "ELDRP7.h"
#endif
#ifdef SPOL
#include "SPOLRP7.h"
#endif

timoff()
{
   unsigned short *timoff;
 /* map Old Timing Module in VME Extended Space */ 
   timoff = (unsigned short *)(TIMBASE + TIMOFF);
   *timoff = 0x0000;
}
