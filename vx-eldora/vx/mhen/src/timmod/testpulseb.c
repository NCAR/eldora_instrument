/*
 *	$Id$
 *
 *	Module:	TESTPULSEB	 
 *	Original Author: Shawn B. Copeland 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1992/09/14  20:46:27  shawn
 * added prf as a parameter, for error checking on end-of-testpulse position.
 * If the end of testpulse occurs after 99% of the inter-pulse spacing
 * specified by prf, then the program will abort without programming new
 * testpulse parameters.
 *
 * Revision 1.1  1992/09/13  23:43:20  shawn
 * Initial revision
 *
 * description:
 *
  Purpose:       Program ELDORA timing module rev B test pulse parameters.
  Arguments:     prf        int, prf of chip/preknock that triggers testpulse;
                            This is to provide error checking to abort if the
			    requested testpulse position overruns the prf of
			    the selected chip/preknock.
                 foraft     int, 1 for forward, 0 for aft.
                 mux        int, mux value to trigger test pulse:
		            0 - CHIP(0)
			    1 - CHIP(1)
			    2 - CHIP(2)
			    3 - CHIP(3)
			    4 - CHIP(4)
			    5 - PK(0)
		            8 - CHIP(5)
			    9 - CHIP(6)
			    10 - CHIP(7)
			    11 - CHIP(8)
			    12 - CHIP(9)
			    13 - PK(1)
		 dist       int, meters to beginning of testpulse.
		 width      int, test pulse width in meters.
  Date Written:  09/11/92
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "ELDRP7.h"

extern int Silent;
void testpulseb(prf,foraft,mux,dist,width)
     
     int prf,foraft,mux,dist,width;
     
{
  unsigned int realdist,realwidth,endpulse,prfmeters;
  unsigned short *timmux,*timdist,*timwidth;
  unsigned short distcounts,widthcounts;

/* Pointers to Timing Module locations  */

if (foraft == 0)
  {   /* AFT */
      timmux   = (unsigned short *)(TIMBASE + TIMATPMUX);  /* Mux */
      timdist  = (unsigned short *)(TIMBASE + TIMATPDIST); /* Distance Countr*/
      timwidth = (unsigned short *)(TIMBASE + TIMATPWIDT); /* Width Counter*/
  }
else if (foraft == 1)
  {   /* FOR */
      timmux   = (unsigned short *)(TIMBASE + TIMFTPMUX);  /* Mux */
      timdist  = (unsigned short *)(TIMBASE + TIMFTPDIST); /* Distance Countr*/
      timwidth = (unsigned short *)(TIMBASE + TIMFTPWIDT); /* Width Counter*/
  }
else
  {   /* ERROR */
      if(!Silent)
	{
	    printf("testpulseb: incorrect value for foraft (%d) passed.\n",foraft);
	    printf("testpulseb: aborting without programming test pulse values.\n");
	}
      return;
  }

  /* print out usage info */
  if(!Silent)
    {
	printf("Hello, this is testpulseb.  Usage:\n");
	printf("testpulseb prf[Hz],foraft[1,0],mux,dist[m],width[m]\n\n");
	printf("values actually passed to testpulseb==> %d,%d,%d,%d,%d\n",prf,foraft,mux,dist,width);
    }
  /* do minor error checking */
  if ((mux<0) || (mux==6) || (mux==7) || (mux > 13))
    {
	if(!Silent)
	  {
	      printf("testpulseb: invalid mux value (%d) passed.\n",mux);
	      printf("testpulseb: aborting without programming test pulse values.\n");
	  }
      return;
    }

  /* compute derived parameters */
  distcounts = dist/10;   /* 10 meters per 15MHz count */
  widthcounts = width/10; /* 10 meters per 15MHz count */
  realdist = distcounts * 10;
  realwidth = widthcounts * 10;
  endpulse = realdist + realwidth;
  prfmeters = (1.0/(float) prf) * 150000000.0;

  /* check if end of testpulse is greater than 99% of a prt */
  if ( endpulse > (0.99 * prfmeters) )
    {
	if(!Silent)
	  {
	      printf("testpulseb: end of testpulse (%d[m]) occurs after\n",endpulse);
	      printf("            next occurance of selected chip or preknock.\n");
	      printf("testpulseb: The end of the testpulse must be within 99%%\n");
	      printf("            of the interpulse distance (%d[m]).\n",prfmeters);
	      printf("testpulseb: In other words, the end of the testpulse is\n");
	      printf("            positioned too far out.\n");
	      printf("testpulseb: aborting without programming testpulse values.\n");
	  }
	return;
    }	

  /* reset testpulse circuitry to fix intermittent startup problem */

  *timmux = 0x16;
  *timmux = 0x06;

  /* write actual values out */

  *timmux   = mux + 0x10; /* disable testpulse reset */
  *timdist  = distcounts;
  *timwidth = widthcounts;

  /* ===========SHOW ACTUAL PROGRAMMED VALUES============== */

if(!Silent)
  {
      printf("values actually setup by testpulseb:\n");
      printf("  mux       = %d\n  dist      = %d\n",mux,realdist);
      printf("  width     = %d\n",realwidth);
	    
      printf("testpulseb: finished.\n");
  }
}





