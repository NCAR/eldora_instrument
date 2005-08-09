/*
 *	$Id$
 *
 *	Module:	TESTPULSECW	 
 *	Original Author: Shawn B. Copeland 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/09/14  21:19:39  shawn
 * Initial revision
 *
 *
 * Revision 1.1  1992/09/13  23:43:20  shawn
 * Initial revision
 *
 * description:
 *
  Purpose:       Program ELDORA timing module rev B for a CW testpulse.
  Arguments:     foraft     int, 1 for forward, 0 for aft.
  Date Written:  09/14/92
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "ELDRP7.h"

void testpulsecw(foraft)
     
     int foraft;
     
{
  unsigned short *timmux;

/* Pointers to Timing Module locations  */

if (foraft == 0)
      timmux   = (unsigned short *)(TIMBASE + TIMATPMUX);  /* Aft Mux */
else if (foraft == 1)
      timmux   = (unsigned short *)(TIMBASE + TIMFTPMUX);  /* Forward Mux */
else
  {   /* ERROR */
      printf("testpulsecw: incorrect value for foraft (%d) passed.\n",foraft);
      printf("testpulsecw: aborting without programming a CW test pulse.\n");
      return;
  }

  /* print out usage info */
  printf("Hello, this is testpulsecw.  Usage:\n");
  printf("testpulsecw foraft[1,0]\n\n");
  printf("value actually passed to testpulsecw==> %d\n",foraft);

  /* write to mux with bit7=1, for CW testpulse */
  *timmux   = (unsigned short) 0x0080;

if (foraft == 0)
      printf("testpulsecw: aft testpulse now set for CW mode.\n");
else if (foraft == 1)
      printf("testpulsecw: forward testpulse now set for CW mode.\n");

printf("testpulsecw: finished.\n");
}





