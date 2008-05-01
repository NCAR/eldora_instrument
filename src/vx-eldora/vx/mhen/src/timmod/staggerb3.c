/*
 *	$Id$
 *
 *	Module:	STAGGERB3	 
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
 * Revision 1.1  1991/12/04  18:00:40  eric
 * Initial revision
 *
 *
 description:
 
 Purpose:       Setup timing module for staggered prf.
                Calls load_waveform with appropriate arguments.
		Does not call load_gates.
		Mainly a diagnostic for testing timing module rev. b.
 Arguments:     ratio1,ratio2,prt1,duty
		int ratio1 [na].    Numerator of ratio of first set prt to
		                    second set prt.
		int ratio2 [na].    Denominator of ratio of first set prt
		                    to second set prt.
                int prt1, [usec].   Time between first set of pulses.
		float duty [%].     effective duty cycle of transmitter.
  Date Written:  09/15/92
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "ELDRP7.h"

#define PK_TO_F0_RISE       22  /* in counts  */
#define PK_TO_F0_FALL       19  /* in counts  */

void staggerb3(prt1,ratio1,ratio2,duty)

int prt1,ratio1,ratio2;
float duty;

{
    short num_chips[6],total_pcp,chip_offset[6],chip_width[6];
    char blank_chip[256];
    float repeat_seq;

    short i;
    float totalprt,pulsewidth,realduty;

printf("prt1,ratio1,ratio2,duty ==> %d,%d,%d,%f\n",prt1,ratio1,ratio2,duty);

    /* compute derived parameters */
    realduty = duty / 100.0;
    totalprt = prt1 / 1000000.0 * (ratio1 + ratio2) / ratio1;
    repeat_seq = totalprt * 1000.0;  /* ms of repeat sequence */
    pulsewidth = realduty * totalprt / 2.0;  /* two pulses in a repeat seq */

    /* print out usage info */
    printf("Hello, this is staggerb3.  Usage:\n");
    printf("staggerb3 ratio1,ratio2,prt1[usec],duty[%%],\n\n");
    printf("values actually passed to staggerb3==> %d,%d,%d,%f\n",prt1,ratio1,ratio2,duty);
    printf("computed values: totalprt = %f, realduty = %f, pulsewidth = %f\n\n",totalprt,realduty,pulsewidth);

  /* check for duty cycle > 1% */
  if (duty > 1.0)
    {
      printf("staggerb3: duty cycle must not be more than 1%%.\n");
      printf("staggerb3: requested duty cycle = %f%%.\n",duty);
      printf("staggerb3: aborting without programming timing module...\n\n");
      return;
    } 
  /* check for > 10 usec pulse */
  if (pulsewidth > 0.000010)
    {
      printf("staggerb3: requested pulsewidth is more than 10 usec.\n");
      printf("staggerb3: pulsewidth = %f usec\n",pulsewidth*1000000);
      printf("staggerb3: aborting without programming timing module...\n\n");
      return;
    }

    /* set up three-frequency stepped chirp, staggered prf */
    num_chips[0] = 2;   /* freq 1 */
    num_chips[1] = 2;   /* freq 2 */
    num_chips[2] = 2;   /* freq 3 */
    num_chips[3] = 0;   /* freq 4 */
    num_chips[4] = 0;   /* freq 5 */
    num_chips[5] = 2;   /* preknock */
    
    total_pcp    = ratio1+ratio2;
    
    blank_chip[0] = 0x27;    /* PK, chip(0), chip(1) and chip(2) */
    for (i=1;i<ratio1;i++)
      blank_chip[i] = 0x00;  /* no pk or chips */
    blank_chip[ratio1] = 0x27;    /* PK, chip(0), chip(1) and chip(2) */
    for (i=ratio1+1;i<(ratio1+ratio2+1);i++)
      blank_chip[i] = 0x00;  /* no pk or chips thereafter */

    for (i=0;i<15;i++) printf("i, blank_chip[i] ==> %d, %2x\n",i,blank_chip[i]);

    chip_offset[0] = 5;
    chip_width[0]  = 30;
    chip_offset[1] = 34;
    chip_width[1]  = 30;
    chip_offset[2] = 64;
    chip_width[2]  = 30;
    chip_offset[3] = 0;
    chip_width[3]  = 0;
    chip_offset[4] = 0;
    chip_width[4]  = 0;
    chip_offset[5] = 1;
    chip_width[5]  = 60;
    
    load_waveformb(num_chips,blank_chip,&repeat_seq,&total_pcp,chip_offset,chip_width);
}





  /* ======================DO CHIPS======================= */
  /* quantize chipwidth (for one of the three chips) */
/* chipcounts = realduty * 20000000.0 / prf; */
  /* compute start and end of pre-knock, f0, f1, and f2 */
/* pkstart = 1;
pkend   = pkstart + chipcounts*3 + PK_TO_F0_RISE - PK_TO_F0_FALL;
f0start = pkstart + PK_TO_F0_RISE;
f0end   = f0start + chipcounts;
f1start = f0end;
f1end   = f1start + chipcounts;
f2start = f1end;
f2end   = f2start + chipcounts;
*/
  /* ===========SHOW ACTUAL PROGRAMMED VALUES============== */
/*   printf("values actually setup by staggerb3:\n");
  printf("  prf       = %f\n  duty      = %f%%\n",15000000.0/(pcpn+1),duty);
	    
printf("staggerb3: finished.\n");
*/





