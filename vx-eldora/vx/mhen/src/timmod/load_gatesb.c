/*
 *	$Id$
 *
 *	Module:	LOAD_GATESB	 
 *	Original Author: Shawn B. Copeland
 *      Copyright National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1992/09/16  19:15:53  shawn
 * Removed floating point writes to collator.
 *
 * Revision 1.1  1992/09/15  20:41:00  shawn
 * Initial revision
 *
 * description:

  Purpose:        Load timing module gate RAMs with desired gate sequences.
  Arguments:      num_gates        pointer to an array of 6 shorts.
                  gate_dist1       pointer to array of 2 shorts.
		  gate_dist2       pointer to array of 2 shorts.
                  gate_dist3       pointer to array of 2 shorts.
		  gate_dist4       pointer to array of 2 shorts.
                  gate_dist5       pointer to array of 2 shorts.

  Date Written:   09/15/92  (copied very old, non-functional load_gates and
                             re-wrote).
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "ELDRP7.h"

void load_gatesb(short num_gates[],short gate_dist1[],short gate_dist2[],short gate_dist3[],short gate_dist4[],short gate_dist5[])

{
    unsigned short *timgate0,*timgate1,*timgate2,*timgate3,*timgate4;
    unsigned short i,gateindex,gatevalue;
    
    timgate0   = (unsigned short *)(TIMBASE + TIMGATE0); /* Gate RAM 0 */
    timgate1   = (unsigned short *)(TIMBASE + TIMGATE1); /* Gate RAM 1 */
    timgate2   = (unsigned short *)(TIMBASE + TIMGATE2); /* Gate RAM 2 */
    timgate3   = (unsigned short *)(TIMBASE + TIMGATE3); /* Gate RAM 3 */
    timgate4   = (unsigned short *)(TIMBASE + TIMGATE4); /* Gate RAM 4 */
    
    for (i=0; i<5; i++)
      /* check for reasonable number of gates */
      if ((num_gates [i] < 0) || (num_gates[i] > 1023))
	{
	    printf("load_gatesb: Requested number of gates [%d]",num_gates[i]);
	    printf("for frequency %d\n",i);
	    printf("             is not between 0 and 1023.\n");
	    printf("load_gatesb: Aborting without programming timmod...\n\n");
	    return;
	}
   
    /* check for unreasonable first gates */
    if ((gate_dist1[0] < 8) || (gate_dist1[0] > 2046))
      {
	  printf("load_gatesb: first gate must be between 8 counts and 2046 counts.\n");
	  printf("load_gatesb: requested first gate for freq 1 was %d counts.\n",gate_dist1[0]);
	  printf("load_gatesb: aborting without programming timmod...\n\n");
	  return;
      }
    if ((gate_dist2[0] < 8) || (gate_dist2[0] > 2046))
      {
	  printf("load_gatesb: first gate spacing must be between 8 counts and 2046 counts.\n");
	  printf("load_gatesb: requested first gate for freq 2 was %d counts.\n",gate_dist2[0]);
	  printf("load_gatesb: aborting without programming timing module...\n\n");
	  return;
      }
    if ((gate_dist3[0] < 8) || (gate_dist3[0] > 2046))
      {
	  printf("load_gatesb: first gate must be between 8 counts and 2046 counts.\n");
	  printf("load_gatesb: requested first gate for freq 3 was %d counts.\n",gate_dist3[0]);
	  printf("load_gatesb: aborting without programming timing module...\n\n");
	  return;
      }
    if ((gate_dist4[0] < 8) || (gate_dist4[0] > 2046))
      {
	  printf("load_gatesb: first gate must be between 8 counts and 2046 counts.\n");
	  printf("load_gatesb: requested first gate for freq 4 was %d counts.\n",gate_dist4[0]);
	  printf("load_gatesb: aborting without programming timing module...\n\n");
	  return;
      }
    if ((gate_dist5[0] < 8) || (gate_dist5[0] > 2046))
      {
	  printf("load_gatesb: first gate must be between 8 counts and 2046 counts.\n");
	  printf("load_gatesb: requested first gate for freq 5 was %d counts.\n",gate_dist5[0]);
	  printf("load_gatesb: aborting without programming timing module...\n\n");
	  return;
      }

    /* check for unreasonable gate spacing */
    if ((gate_dist1[1] < 8) || (gate_dist1[1] > 800))
      {
	  printf("load_gatesb: gate spacing must be between 8 counts and 800 counts.\n");
	  printf("load_gatesb: requested spacing for freq 1 was %d counts.\n",gate_dist1[1]);
	  printf("load_gatesb: aborting without programming timing module...\n\n");
	  return;
      }
    if ((gate_dist2[1] < 8) || (gate_dist2[1] > 800))
      {
	  printf("load_gatesb: gate spacing must be between 8 counts and 800 counts.\n");
	  printf("load_gatesb: requested spacing for freq 2 was %d counts.\n",gate_dist2[1]);
	  printf("load_gatesb: aborting without programming timing module...\n\n");
	  return;
      }
    if ((gate_dist3[1] < 8) || (gate_dist3[1] > 800))
      {
	  printf("load_gatesb: gate spacing must be between 8 counts and 800 counts.\n");
	  printf("load_gatesb: requested spacing for freq 3 was %d counts.\n",gate_dist3[1]);
	  printf("load_gatesb: aborting without programming timing module...\n\n");
	  return;
      }
    if ((gate_dist4[1] < 8) || (gate_dist4[1] > 800))
      {
	  printf("load_gatesb: gate spacing must be between 8 counts and 800 counts.\n");
	  printf("load_gatesb: requested spacing for freq 4 was %d counts.\n",gate_dist4[1]);
	  printf("load_gatesb: aborting without programming timing module...\n\n");
	  return;
      }
    if ((gate_dist5[1] < 8) || (gate_dist5[1] > 800))
      {
	  printf("load_gatesb: gate spacing must be between 8 counts and 800 counts.\n");
	  printf("load_gatesb: requested spacing for freq 5 was %d counts.\n",gate_dist5[1]);
	  printf("load_gatesb: aborting without programming timing module...\n\n");
	  return;
      }
    
    /* ======================DO GATES======================= */
    /* Load first gate count-up value */
    gatevalue = 0x1003 - gate_dist1[0];
    *timgate0 = gatevalue;
    gatevalue = 0x1003 - gate_dist2[0];
    *timgate1 = gatevalue;
    gatevalue = 0x1003 - gate_dist3[0];
    *timgate2 = gatevalue;
    gatevalue = 0x1003 - gate_dist4[0];
    *timgate3 = gatevalue;
    gatevalue = 0x1003 - gate_dist5[0];
    *timgate4 = gatevalue;
    timgate0++;
    timgate1++;
    timgate2++;
    timgate3++;
    timgate4++;

    /* Load the rest of the gate count-up values for freq 1*/
    gatevalue = 0x1002 - gate_dist1[1];
    for (gateindex=1; gateindex<num_gates[0]; gateindex++)
      {
	  *timgate0 = gatevalue;
	  timgate0++;
	  if(timgate0 > (unsigned short *) TIMBASE+TIMGATE0+4090)
	    {
		printf("load_gatesb: GATE RAM index computation error.\n");
		printf("load_gatesb: aborting without fully programming timing module.\n\n");
		return;
	    }
      }
    *timgate0 = 0x0000;    /* Load terminal value */

    /* Load the rest of the gate count-up values for freq 2*/
    gatevalue = 0x1002 - gate_dist2[1];
    for (gateindex=1; gateindex<num_gates[1]; gateindex++)
      {
	  *timgate1 = gatevalue;
	  timgate1++;
	  if(timgate1 > (unsigned short *) TIMBASE+TIMGATE1+4090)
	    {
		printf("load_gatesb: GATE RAM index computation error.\n");
		printf("load_gatesb: aborting without fully programming timing module.\n\n");
		return;
	    }
      }
    *timgate1 = 0x0000;    /* Load terminal value */

    /* Load the rest of the gate count-up values for freq 3*/
    gatevalue = 0x1002 - gate_dist3[1];
    for (gateindex=1; gateindex<num_gates[2]; gateindex++)
      {
	  *timgate2 = gatevalue;
	  timgate2++;
	  if(timgate2 > (unsigned short *) TIMBASE+TIMGATE2+4090)
	    {
		printf("load_gatesb: GATE RAM index computation error.\n");
		printf("load_gatesb: aborting without fully programming timing module.\n\n");
		return;
	    }
      }
    *timgate2 = 0x0000;    /* Load terminal value */

    /* Load the rest of the gate count-up values for freq 4*/
    gatevalue = 0x1002 - gate_dist4[1];
    for (gateindex=1; gateindex<num_gates[3]; gateindex++)
      {
	  *timgate3 = gatevalue;
	  timgate3++;
	  if(timgate3 > (unsigned short *) TIMBASE+TIMGATE3+4090)
	    {
		printf("load_gatesb: GATE RAM index computation error.\n");
		printf("load_gatesb: aborting without fully programming timing module.\n\n");
		return;
	    }
      }
    *timgate3 = 0x0000;    /* Load terminal value */

    /* Load the rest of the gate count-up values for freq 5*/
    gatevalue = 0x1002 - gate_dist5[1];
    for (gateindex=1; gateindex<num_gates[4]; gateindex++)
      {
	  *timgate4 = gatevalue;
	  timgate4++;
	  if(timgate4 > (unsigned short *) TIMBASE+TIMGATE4+4090)
	    {
		printf("load_gatesb: GATE RAM index computation error.\n");
		printf("load_gatesb: aborting without fully programming timing module.\n\n");
		return;
	    }
      }
    *timgate4 = 0x0000;    /* Load terminal value */
    
    /* ===========SHOW ACTUAL PROGRAMMED VALUES============== */
    printf("values actually setup by load_gatesb:\n");
    printf("  FREQ 1: firstgate = %f[m]  gatespace = %f[m]",gate_dist1[0]*2.5,gate_dist1[1]*2.5);
    printf("  FREQ 2: firstgate = %f[m]  gatespace = %f[m]",gate_dist2[0]*2.5,gate_dist2[1]*2.5);
    printf("  FREQ 3: firstgate = %f[m]  gatespace = %f[m]",gate_dist3[0]*2.5,gate_dist3[1]*2.5);
    printf("  FREQ 4: firstgate = %f[m]  gatespace = %f[m]",gate_dist4[0]*2.5,gate_dist4[1]*2.5);
    printf("  FREQ 5: firstgate = %f[m]  gatespace = %f[m]",gate_dist5[0]*2.5,gate_dist5[1]*2.5);
    
    printf("load_gatesb: finished.\n");
    
}




