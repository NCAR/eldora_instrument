/*
 *	$Id$
 *
 *	Module:	TIMINGB	 
 *	Original Author: Shawn B. Copeland 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/09/11  20:02:04  shawn
 * Initial revision
 *
 *
 * Revision 1.1  1991/12/04  19:01:58  eric
 * Initial revision
 *
 *
 * description:
 *
  Purpose:       Program ELDORA timing module rev B to generate a single prf
		 on F0, and a pre-delayed preknock.
		 Also programs N number of uniform-spaced gates on GATE(0).
  Arguments:     prf        int, prf in Hz.
                 numgates   int, number of gates.
		 gatespace  int, gate spacing in meters.
		 duty       float, duty cycle in percent.
  Date Written:  09/11/92
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";


#include "ELDRP7.h"
#include "coll_dpramdefs.h"

#define PK_TO_F0_RISE       22  /* in counts  */
#define PK_TO_F0_FALL       19  /* in counts  */

void timingb(prf,numgates,gatespace,duty)
     
     int prf,numgates,gatespace;
     float duty;
     
{
  float floatgate,gatetime,prt,pulsewidth,realduty;
  unsigned short *timgate0,*timpcpn,*timblank,*timchip,*timoff;
  unsigned short pcpn,chipcounts,gatecounts;
  unsigned short pkstart,pkend,f0start,f0end;
  unsigned short maxindex,chipindex,chipramvalue;
  unsigned short gateindex,gatevalue;

/* Pointers to Collator */
  unsigned long  *col_first,*col_gate;
  col_first  = (unsigned long *) (COL0BASE + COL_RZERO);           /* Col. first-gate   */
  col_gate   = (unsigned long *) (COL0BASE + COL_GATE_SPACE);      /* Col. gate-spacing */

  timgate0   = (unsigned short *)(TIMBASE + TIMGATE0); /* Gate RAM 0 */
  timpcpn    = (unsigned short *)(TIMBASE + TIMPCPN);  /* N for PCP Counter */
  timblank   = (unsigned short *)(TIMBASE + TIMBLANK); /* Blanking RAM */
  timchip    = (unsigned short *)(TIMBASE + TIMCHIP);  /* Chip RAM */
  timoff     = (unsigned short *)(TIMBASE + TIMOFF);   /* Stop address */

  /* compute derived parameters */
  prt      = (1.0 / prf);
  realduty = duty / 100;
  pulsewidth = realduty / prf;
  gatecounts = (float) gatespace / 2.5;

  /* print out usage info at Eric's request */
  printf("Hello, this is timingb.  Usage:\n");
  printf("timingb prf[Hz],numgates,gatespace[m],duty[%%]\n\n");
  printf("values actually passed to timingb==> %d,%d,%d,%f\n",prf,numgates,gatespace,duty);

  /* do minor error checking */
  if (prf < 250)
    {
      printf("timingb: prf must be greater than 250 Hz.\n");
      printf("timingb: aborting without programming timingb module...\n\n");
      return;
    }
  if (prf > 20000)
    {
      printf("timingb: requested prf is greater than 20000.\n");
      printf("timingb: aborting without programming timingb module...\n\n");
      return;
    }
  /* check for reasonable number of gates */
  if (numgates > 1024)
    {
      printf("timingb: requested number of gates is greater than 1024.\n");
      printf("timingb: aborting without programming timingb module...\n\n");
      return;
    }
  /* check for unreasonable gate spacing */
  if ((gatespace < 30) | (gatespace > 1200))
    {
      printf("timingb: gate spacing must be between 30 and 1200 meters.\n");
      printf("timingb: requested spacing was %d meters.\n",gatespace);
      printf("timingb: aborting without programming timingb module...\n\n");
      return;
    }
  /* check for overflow of gates for requested prf */
  gatetime = (numgates * gatespace) / 150000000.0;  /* in seconds */
  if (gatetime > 0.98*prt)
    {
      printf("timingb: gates fill more than 98%% of a prt.\n");
      printf("timingb: time for all gates = %f usec\n",gatetime*1000000);
      printf("timingb: time in one prf    = %f usec\n",prt*1000000);
      printf("timingb: aborting without programming timingb module...\n\n");
      return;
    }
  /* check for duty cycle > 1% */
  if (duty > 1.0)
    {
      printf("timingb: duty cycle must not be more than 1%%.\n");
      printf("timingb: requested duty cycle = %f%%.\n",duty);
      printf("timingb: aborting without programming timingb module...\n\n");
      return;
    } 
  /* check for > 10 usec pulse */
  if (pulsewidth > 0.000010)
    {
      printf("timingb: requested pulsewidth is more than 10 usec.\n");
      printf("timingb: pulsewidth = %f usec\n",pulsewidth*1000000);
      printf("timingb: aborting without programming timingb module...\n\n");
      return;
    }

  *timoff = 0x0000;  /* shut off module */

  /* write gate spacing in km (in at&t floating point) to collator DPRAM */
  floatgate  = (float) gatespace/1000;
  *col_first = i3e2dsp(floatgate);
  *col_gate  = i3e2dsp(floatgate);

  /* Compute N for needed PCP, write it to timingb module */
  pcpn = 15000000.0 / prf;
  pcpn = pcpn - 1;
  *timpcpn = pcpn;

  /* Write Blanking RAM sequence */
  *timblank = (unsigned short) 0x0403;

  /* ======================DO CHIPS======================= */
  /* quantize chipwidth */
  chipcounts = realduty * 60000000.0 / prf;
  /* compute start and end of pre-knock, and f0 */
  pkstart = 1;
  pkend   = pkstart + chipcounts + PK_TO_F0_RISE - PK_TO_F0_FALL;
  f0start = pkstart + PK_TO_F0_RISE;
  f0end   = f0start + chipcounts;

  /* find end of sequence */
  maxindex = pkend;
  if (f0end > maxindex) maxindex = f0end;

  /* error checking */
  if (maxindex > 4090)
    {
      printf("timingb: index computation error.  CHIP RAM bounds exceeded.\n");
      printf("timingb: aborting without fully programming timingb module.\n\n");
      return;
    }

  /* Load chip RAM */
  *timchip = 0x8000;  /* Load initial, special value */
  timchip++;
  for (chipindex = 1; chipindex<maxindex; chipindex++)
    {
      chipramvalue = 0x8000;
      if ( (chipindex >= pkstart) & (chipindex < pkend) )
	chipramvalue = chipramvalue | 0x0400;
      if ( (chipindex >= f0start) & (chipindex < f0end) )
	chipramvalue = chipramvalue | 0x0001;
      *timchip = chipramvalue;
      timchip++;
      if (timchip > (unsigned short *) TIMBASE+TIMCHIP+4090)
	{
	  printf("timingb: CHIP RAM index computation error.\n");
	  printf("timingb: aborting without fully programming timingb module.\n\n");
	  return;
	}
    }
  /* Write terminal value (twice, as required by timingb module) */
  *timchip = 0x0000;
  timchip++;
  *timchip = 0x0000;

  /* ======================DO GATES======================= */
  /* Load first gate count-up value */
  gatevalue = 0x1003 - (2*gatecounts);
  *timgate0 = gatevalue;
  timgate0++;
  /* Load the rest of the gate count-up values */
  gatevalue = 0x1002 - gatecounts;
  for (gateindex=1; gateindex<numgates; gateindex++)
    {
      *timgate0 = gatevalue;
      timgate0++;
	if(timgate0 > (unsigned short *) TIMBASE+TIMGATE0+4090)
	  {
	    printf("timingb: GATE RAM index computation error.\n");
	    printf("timingb: aborting without fully programming timingb module.\n\n");
	    return;
	  }
    }
  /* Load terminal value */
  *timgate0 = 0x0000;

  /* ===========SHOW ACTUAL PROGRAMMED VALUES============== */
  printf("values actually setup by timingb:\n");
  printf("  prf       = %f\n  numgates  = %d\n",15000000.0/(pcpn+1),numgates);
  printf("  gatespace = %f\n  duty      = %f%%\n",gatecounts*2.5,duty);
	    
printf("timingb: finished.\n");
}





