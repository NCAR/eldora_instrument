/*
 *	$Id$
 *
 *	Module:	TST_TIMING	 
 *	Original Author: Shawn B. Copeland 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1993/05/12  16:01:16  eric
 * Initial revision
 *
 * Revision 1.3  1992/07/31  02:27:20  shawn
 * made first gate twice the distance from leading edge of chip.
 *
 * Revision 1.2  1992/07/30  22:05:03  eric
 * updated with new collator DPRAM locations.
 *
 * Revision 1.1  1991/12/04  19:01:58  eric
 * Initial revision
 *
 *
 * description:
 *
  Purpose:       Utility for NOV91 French IF tests.
                 Program ELDORA timing module to generate a single prf
		 on F0, a test pulse on F1, and a pre-delayed preknock.
		 Also programs N number of uniform-spaced gates on GATE(0).
  Arguments:     prf        int, prf in Hz.
                 numgates   int, number of gates.
		 gatespace  int, gate spacing in meters.
		 duty       float, duty cycle in percent.
		 tp_pos     int, distance to test pulse in meters.
  Date Written:  11/14/91
 *        
 * Added code to original version of timing.c to allow fixed placement of
 * first range gate to occur at approximately 300m from rising edge of
 * BEPRF. This is to test operation of time series processor.
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";


#include "ELDRP7.h"
#include "coll_dpramdefs.h"

#define PK_TO_F0_RISE       22  /* in counts  */
#define PK_TO_F0_FALL       19  /* in counts  */
#define F_GATE             300  /* in meters */
void tst_timing(prf,numgates,gatespace,duty,tp_pos)
     
     int prf,numgates,gatespace,tp_pos;
     float duty;
     
{
  float floatgate,gatetime,prt,pulsewidth,realduty;
  unsigned short *timgate0,*timpcpn,*timblank,*timchip,*timoff;
  unsigned short pcpn,chipcounts,tp_counts,gatecounts;
  unsigned short pkstart,pkend,f0start,f0end,tpstart,tpend;
  unsigned short maxindex,chipindex,chipramvalue;
  unsigned short gateindex,gatevalue;

/* Pointers to Collator */
  unsigned long  *col_first,*col_gate;
  col_first  = (unsigned long *) (COL0BASE + COL_RZERO);           /* Col. first-gate   */
  col_gate   = (unsigned long *) (COL0BASE + COL_GATE_SPACE);      /* Col. gate-spacing */

  timgate0   = (unsigned short *)(TIMBASE - STD_BASE + EXTD_BASE + TIMGATE0); /* Gate RAM 0 */
  timpcpn    = (unsigned short *)(TIMBASE - STD_BASE + EXTD_BASE + TIMPCPN);  /* N for PCP Counter */
  timblank   = (unsigned short *)(TIMBASE - STD_BASE + EXTD_BASE + TIMBLANK); /* Blanking RAM */
  timchip    = (unsigned short *)(TIMBASE - STD_BASE + EXTD_BASE + TIMCHIP);  /* Chip RAM */
  timoff     = (unsigned short *)(TIMBASE - STD_BASE + EXTD_BASE + TIMOFF);   /* Stop address */

  /* compute derived parameters */
  prt      = (1.0 / prf);
  realduty = duty / 100;
  pulsewidth = realduty / prf;
  gatecounts = (float) gatespace / 2.5;

  /* print out usage info at Eric's request */
  printf("Hello, this is timing.  Usage:\n");
  printf("timing prf[Hz],numgates,gatespace[m],duty[%%],tp_pos[m]\n\n");
  printf("values actually passed to timing==> %d,%d,%d,%f,%d\n",prf,numgates,gatespace,duty,tp_pos);

  /* do minor error checking */
  if (prf < 250)
    {
      printf("timing: prf must be greater than 250 Hz.\n");
      printf("timing: aborting without programming timing module...\n\n");
      return;
    }
  if (prf > 20000)
    {
      printf("timing: requested prf is greater than 20000.\n");
      printf("timing: aborting without programming timing module...\n\n");
      return;
    }
  /* check for reasonable number of gates */
 /* try commenting this out
  if (numgates > 1024)
    {
      printf("timing: requested number of gates is greater than 1024.\n");
      printf("timing: aborting without programming timing module...\n\n");
      return;
    }
 */
  /* check for unreasonable gate spacing */
  if ((gatespace < 30) | (gatespace > 1200))
    {
      printf("timing: gate spacing must be between 30 and 1200 meters.\n");
      printf("timing: requested spacing was %d meters.\n",gatespace);
      printf("timing: aborting without programming timing module...\n\n");
      return;
    }
  /* check for test pulse out of range */
  if (tp_pos > 9000)
    {
      printf("timing: test pulse position must be less than 9000 meters.\n");
      printf("timing: aborting without programming timing module...\n\n");
      return;
    }
  /* check for overflow of gates for requested prf */
  gatetime = (numgates * gatespace) / 150000000.0;  /* in seconds */
  if (gatetime > 0.98*prt)
    {
      printf("timing: gates fill more than 98%% of a prt.\n");
      printf("timing: time for all gates = %f usec\n",gatetime*1000000);
      printf("timing: time in one prf    = %f usec\n",prt*1000000);
      printf("timing: aborting without programming timing module...\n\n");
      return;
    }
  /* check for duty cycle > 1% */
  if (duty > 1.0)
    {
      printf("timing: duty cycle must not be more than 1%%.\n");
      printf("timing: requested duty cycle = %f%%.\n",duty);
      printf("timing: aborting without programming timing module...\n\n");
      return;
    } 
  /* check for > 10 usec pulse */
  if (pulsewidth > 0.000010)
    {
      printf("timing: requested pulsewidth is more than 10 usec.\n");
      printf("timing: pulsewidth = %f usec\n",pulsewidth*1000000);
      printf("timing: aborting without programming timing module...\n\n");
      return;
    }

  *timoff = 0x0000;  /* shut off module */

  /* write gate spacing in km (in at&t floating point) to collator DPRAM */
  floatgate  = (float) gatespace/1000;
  *col_first = i3e2dsp(floatgate);
  *col_gate  = i3e2dsp(floatgate);

  /* Compute N for needed PCP, write it to timing module */
  pcpn = 15000000.0 / prf;
  pcpn = pcpn - 1;
  *timpcpn = pcpn;

  /* Write Blanking RAM sequence */
  *timblank = (unsigned short) 0x0403;

  /* ======================DO CHIPS======================= */
  /* quantize chipwidth and test pulse position */
  chipcounts = realduty * 60000000.0 / prf;
  tp_counts = tp_pos / 2.5;
  /* compute start and end of pre-knock, f0, and test pulse (f1) */
  pkstart = 1;
  pkend   = pkstart + chipcounts + PK_TO_F0_RISE - PK_TO_F0_FALL;
  f0start = pkstart + PK_TO_F0_RISE;
  f0end   = f0start + chipcounts;
  tpstart = tp_counts + PK_TO_F0_RISE - (chipcounts / 2);
  tpend   = tpstart + chipcounts;

  /* find end of sequence */
  maxindex = pkend;
  if (f0end > maxindex) maxindex = f0end;
  if (tpend > maxindex) maxindex = tpend;

  /* error checking */
  if (maxindex > 4090)
    {
      printf("timing: index computation error.  CHIP RAM bounds exceeded.\n");
      printf("timing: aborting without fully programming timing module.\n\n");
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
      if ( (chipindex >= tpstart) & (chipindex < tpend) )
	chipramvalue = chipramvalue | 0x0002;
      *timchip = chipramvalue;
      timchip++;
      if (timchip > (unsigned short *) TIMBASE - STD_BASE + EXTD_BASE+TIMCHIP+4090)
	{
	  printf("timing: CHIP RAM index computation error.\n");
	  printf("timing: aborting without fully programming timing module.\n\n");
	  return;
	}
    }
  /* Write terminal value (twice, as required by timing module) */
  *timchip = 0x0000;
  timchip++;
  *timchip = 0x0000;

  /* ======================DO GATES======================= */
  /* Load first gate count-up value */
  gatevalue = 0x1003 - (F_GATE/2.5 + 0.5); /* should put first gate out
                                              F_GATE meters from BEPRF */
  *timgate0 = gatevalue;
  timgate0++;
  /* Load the rest of the gate count-up values */
  gatevalue = 0x1002 - gatecounts;
  for (gateindex=1; gateindex<numgates; gateindex++)
    {
      *timgate0 = gatevalue;
      timgate0++;
	if(timgate0 > (unsigned short *) TIMBASE - STD_BASE + EXTD_BASE+TIMGATE0+4090)
	  {
	    printf("timing: GATE RAM index computation error.\n");
	    printf("timing: aborting without fully programming timing module.\n\n");
	    return;
	  }
    }
  /* Load terminal value */
  *timgate0 = 0x0000;

  /* ===========SHOW ACTUAL PROGRAMMED VALUES============== */
  printf("values actually setup by timing:\n");
  printf("  prf       = %f\n  numgates  = %d\n",15000000.0/(pcpn+1),numgates);
  printf("  gatespace = %f\n  duty      = %f%%\n",gatecounts*2.5,duty);
  printf("  testpulse = %f\n",tp_counts*2.5);
	    
printf("timing: finished.\n");
  
}





