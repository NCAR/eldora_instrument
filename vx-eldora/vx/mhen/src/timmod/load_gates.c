/*
 *	$Id$
 *
 *	Module:	LOAD_GATES	 
 *	Original Author: Shawn B. Copeland
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description:

  Purpose:        Load timing module gate RAMs with desired gate sequences.
  Arguments:      uniform_gates    pointer to an array of unsigned chars.
                  cell_total       pointer to array of unsigned shorts.
		  cell_dist        pointer to array of unsigned shorts.

  Date Written:   12/17/90
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";


#include "/ELDORA/vx/mhen/include/ELDRP7.h"

void load_gates(uniform_gates,cell_total,cell_dist)

unsigned char uniform_gates[MAXRADARS];
unsigned short cell_total[MAXRADARS],cell_dist[][MAXGATES];
{
  unsigned short *gateloc[10],loadvalue;
  short i,freq,gate;
  
  gateloc[0]     = (unsigned short *)(TIMBASE + TIMGATE0);
  gateloc[1]     = (unsigned short *)(TIMBASE + TIMGATE1);
  gateloc[2]     = (unsigned short *)(TIMBASE + TIMGATE2);
  gateloc[3]     = (unsigned short *)(TIMBASE + TIMGATE3);
  gateloc[4]     = (unsigned short *)(TIMBASE + TIMGATE4);
  gateloc[5]     = (unsigned short *)(TIMBASE + TIMGATE5);
  gateloc[6]     = (unsigned short *)(TIMBASE + TIMGATE6);
  gateloc[7]     = (unsigned short *)(TIMBASE + TIMGATE7);
  gateloc[8]     = (unsigned short *)(TIMBASE + TIMGATE8);
  gateloc[9]     = (unsigned short *)(TIMBASE + TIMGATE9);
  
  for (freq=0; freq<MAXRADARS; freq++)
    {
      if (uniform_gates[freq] = 1)          /* uniform gates? */
	{
	  loadvalue = 0x1002 - cell_dist[freq][0];  /* calculate first gate */
	  if (loadvalue < 0x800)              /* error checking */
	    {
	      printf("First gate too large... freq= %d.\n",freq); 
	      return;
	    }
	  if (loadvalue > 0xFFC)              /* error checking */
	    {
	      printf("Second gate too small... freq= %d./n",freq); 
	      return;
	    }
	  *(gateloc[freq]) = loadvalue;  /* load first gate */
	  gateloc[freq]++;
	  loadvalue = 0x1002 - cell_dist[freq][1]; /* get value for rest */
	  if (loadvalue < 0x800)              /* error checking */
	    {
	      printf("Gate too large... freq= %d, gate = %d\n",freq,1); 
	      return;
	    }
	  if (loadvalue > 0xFFC)              /* error checking */
	    {
	      printf("Gate too small... freq= %d, gate = %d\n",freq,1); 
	      return;
	    }
	  for (gate=1; gate<cell_total[freq]; gate++) /* load the rest */
	    {
	      *(gateloc[freq]) = loadvalue;
	      gateloc[freq]++;
	    }
	  *(gateloc[freq]) = 0x0000;  /* load terminal value */
	}
      else                                  /* non-uniform gates */
	{
	  for (gate=0; gate < cell_total[freq]; gate++)
	    {
	      loadvalue = 0x1002 - cell_dist[freq][gate];
	      if (loadvalue < 0x800)              /* error checking */
		{
		  printf("Gate too large... freq= %d, gate = %d\n",freq,gate); 
		  return;
		}
	      if (loadvalue > 0xFFC)              /* error checking */
		{
		  printf("Gate too small... freq= %d, gate = %d\n",freq,gate); 
		  return;
		}
	      *(gateloc[freq]) = loadvalue;
	      gateloc[freq]++;
	    }
	  *(gateloc[freq]) = 0x0000;  /* load terminal value */
	}
    }
}




