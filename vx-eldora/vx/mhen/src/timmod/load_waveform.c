/*
 *	$Id$
 *
 *	Module:	LOAD_WAVEFORM	 
 *	Original Author: Shawn B. Copeland
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description:
 *
  Purpose:       Program ELDORA Timing Module for a desired waveform.
                 (Does not load gate generators).
  Arguments:     num_chips      pointer to array of 6 shorts.
                 chip_offset    pointer to array of 6xMAXCHIPS shorts.
		 chip_width     pointer to array of 6xMAXCHIPS shorts.
		 total_pcp      pointer to a short.
		 blank_chip     pointer to array of MAXPCP unsigned shorts.
		 repeat_seq     pointer to a float.

  Date Written:  12/11/90...(from copy of onefreq.c)
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";


#include "/ELDORA/vx/mhen/include/ELDRP7.h"

#define NUMFREQ 5

void load_waveform(num_chips,chip_offset,chip_width,total_pcp,blank_chip,repeat_seq)
     
     short num_chips[],chip_offset[],chip_width[];
     short *total_pcp;
     unsigned short blank_chip[];
     float *repeat_seq;
     
{
  static unsigned short *blankloc,*nloc,*chiploc,chipvals[4096],blankvals[256];
  static unsigned short i,freq,chipstart,chipstop,laststop,lastpcp,blanknib;

  nloc        = (unsigned short *)(TIMBASE + TIMPCPN);
  blankloc    = (unsigned short *)(TIMBASE + TIMBLANK);
  chiploc     = (unsigned short *)(TIMBASE + TIMCHIP);
  
  /* load N for desired PCP */
  *nloc = (unsigned short) (*repeat_seq * 15000 / *total_pcp);
  
  /* GENERATE CHIP VALUES */
  /* Build array to load into chip RAM */
  chipvals[0] = 0x8000;   /* special initial value */
  for (i=1; i<4095; i++)   /* zero out array (leave EOS in all locations) */
    {
      chipvals[i] = 0x0000;
    }

  laststop = 0;  /* initialize "last count" value to zero */
  
  for (freq=0; freq<NUMFREQ; freq++)  /* step through frequencies */
    {
      chipstart = chip_offset[freq]+1;
      chipstop  = chipstart+chip_width[freq];
      if (chipstop > laststop) laststop = chipstop;
      for (i=chipstart; i<chipstop; i++)
	{
	  chipvals[i] = chipvals[i] ^ 0x21<<freq; /* load for and aft */
	}
    }

  freq = 5;  /* do preknock */
  chipstart = chip_offset[freq]+1;
  chipstop  = chipstart+chip_width[freq];
  if (chipstop > laststop) laststop = chipstop;
  for (i=chipstart; i<chipstop; i++)
      chipvals[i] = chipvals[i] ^ 0xc00; /* load pk's */

  /* set STOPCT* bit until end of sequence */
  for (i=1; i<laststop; i++) chipvals[i] = chipvals[i] ^ 0x8000; 

  /* write array to timing module Chip RAM */
  for (i=0; i<4095; i++)
    {
      *chiploc = (unsigned short) chipvals[i];
      chiploc++;
    }
  
  /* GENERATE BLANKING VALUES */
  /* Build array to load into blanking RAM */
  for (i=0; i<MAXPCP; i++)   /* zero out array (leave EOS in all locations) */
    {
      blankvals[i] = 0x0000;
    }
  
  for (i=0; i<*total_pcp; i++)  /* step through all used blanking values */
    {
      if (blank_chip[i] & 0x0020 == 0x0020) blankvals[i] = 0x0C00; /*shift pk*/
      blanknib = blank_chip[i] & 0x001f;  /* get lower 5 blanking bits */
      blankvals[i] = blankvals[i] ^ blanknib;  /* put them in the array */
      blankvals[i] = blankvals[i] ^ (blanknib << 5); /* repeat 5 bits higher */
    }
  
  /* set STOPCT* bit until one less than end of sequence */
  (*total_pcp)--;
  for (i=0; i<*total_pcp; i++) blankvals[i] = blankvals[i] ^ 0x8000;

  /* write array to timing module blanking RAM */
  for (i=0; i<MAXPCP; i++)
    {
      *blankloc = (unsigned short) blankvals[i];
      blankloc++;
    }
}
