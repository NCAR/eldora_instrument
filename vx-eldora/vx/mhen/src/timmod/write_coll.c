/*
 *	$Id$
 *
 *	Module:	WRITE_COLL	 
 *	Original Author: Shawn B. Copeland 
 *      Copyright National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/09/11  20:02:04  shawn
 * Initial revision
 *
 *
 * description:
 *
  Purpose:       Write gate spacing to collator DPRAM.
  Arguments:     firstgate  int, distance to first gate in meters.
		 gatespace  int, subsequent gate spacing in meters.
  Date Written:  09/16/92
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "ELDRP7.h"
#include "coll_dpramdefs.h"

void write_coll(firstgate,gatespace)
     
     int firstgate,gatespace;
     
{
  float floatgate;

/* Pointers to Collator */
  unsigned long  *col_first,*col_gate;
  col_first  = (unsigned long *)(COL0BASE+COL_RZERO);     /* Col. first-gate */
  col_gate   = (unsigned long *)(COL0BASE+COL_GATE_SPACE);/* Col. gate-space */

  /* print out usage info */
  printf("Hello, this is write_coll.  Usage:\n");
  printf("write_coll firstgate[m],gatespace[m]\n\n");
  printf("values actually passed to write_coll==>%d,%d\n",firstgate,gatespace);

  /* check for unreasonable gate spacing */
  if ((gatespace < 20) | (gatespace > 2000))
    {
      printf("write_coll: gate spacing must be between 20 and 2000 meters.\n");
      printf("write_coll: requested spacing was %d meters.\n",gatespace);
      printf("write_coll: aborting without writing to collator...\n\n");
      return;
    }

  /* write gate spacing in km (in at&t floating point) to collator DPRAM */
  floatgate  = (float) gatespace/1000;
  *col_first = i3e2dsp(floatgate);
  *col_gate  = i3e2dsp(floatgate);

  /* ===========SHOW ACTUAL PROGRAMMED VALUES============== */
  printf("values actually setup by write_coll:\n");
  printf("  firstgate = %d\n  gatespace = %d\n",firstgate,gatespace);
	    
printf("write_coll: finished.\n");
}





