/*
 *	$Id$
 *
 *	Module:	LOAD_WAVEFORMB	 
 *	Original Author: Shawn B. Copeland
 *      Copyright National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1991/12/04  18:00:40  eric
 * Initial revision
 *
 *
 description:
 
 Purpose:       Program ELDORA Timing Module for a desired waveform.
                (Does not load gate generators).
 Arguments:     num_chips      pointer to array of 6 shorts.
                blank_chip     pointer to array of 256 chars.
		repeat_seq     pointer to a float.
		total_pcp      pointer to a short.
                chip_offset    pointer to array of 6 shorts.
                chip_width     pointer to array of 6 shorts.
 Date Written:  09/14/92 (copied non-functional load_waveform.c)
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "ELDRP7.h"

void load_waveformb(short num_chips[],char blank_chip[],float *repeat_seq,short *total_pcp,short chip_offset[],short chip_width[])

{
    unsigned short *blankloc,*nloc,*chiploc;
    unsigned short pkstart,f0start,f1start,f2start,f3start,f4start;
    unsigned short pkend,f0end,f1end,f2end,f3end,f4end;
    unsigned short chipramvalue,blankingvalue;
    unsigned short chipindex,maxindex,i;
    
    nloc        = (unsigned short *)(TIMBASE + TIMPCPN);
    blankloc    = (unsigned short *)(TIMBASE + TIMBLANK);
    chiploc     = (unsigned short *)(TIMBASE + TIMCHIP);
    
    /* load N for desired PCP */
    *nloc = (unsigned short) (*repeat_seq * 15000 / *total_pcp);
    
    /* ======================DO CHIPS======================= */
    pkstart = chip_offset[5];
    pkend   = chip_offset[5] + chip_width[5];
    f0start = chip_offset[0];
    f0end   = chip_offset[0] + chip_width[0];
    f1start = chip_offset[1];
    f1end   = chip_offset[1] + chip_width[1];
    f2start = chip_offset[2];
    f2end   = chip_offset[2] + chip_width[2];
    f3start = chip_offset[3];
    f3end   = chip_offset[3] + chip_width[3];
    f4start = chip_offset[4];
    f4end   = chip_offset[4] + chip_width[4];
    
    /* find end of sequence */
    maxindex = pkend;
    if (f0end > maxindex) maxindex = f0end;
    if (f1end > maxindex) maxindex = f1end;
    if (f2end > maxindex) maxindex = f2end;
    if (f3end > maxindex) maxindex = f3end;
    if (f4end > maxindex) maxindex = f4end;
    
    /* error checking */
    if (maxindex > 4090)
      {
	  printf("load_waveformb: index computation error.  CHIP RAM bounds exceeded.\n");
	  printf("load_waveformb: aborting without fully programming timing module.\n\n");
	  return;
      }
    
    /* Load chip RAM */
    *chiploc = 0x8000;  /* Load initial, special value */
    chiploc++;
    for (chipindex = 1; chipindex<maxindex; chipindex++)
      {
	  chipramvalue = 0x8000; /* set STOPCT* bit */
	  if ( (chipindex >= pkstart) & (chipindex < pkend) )
	    chipramvalue = chipramvalue | 0x0400;
	  if ( (chipindex >= f0start) & (chipindex < f0end) )
	    chipramvalue = chipramvalue | 0x0001;
	  if ( (chipindex >= f1start) & (chipindex < f1end) )
	    chipramvalue = chipramvalue | 0x0002;
	  if ( (chipindex >= f2start) & (chipindex < f2end) )
	    chipramvalue = chipramvalue | 0x0004;
	  if ( (chipindex >= f3start) & (chipindex < f3end) )
	    chipramvalue = chipramvalue | 0x0008;
	  if ( (chipindex >= f4start) & (chipindex < f4end) )
	    chipramvalue = chipramvalue | 0x0010;
	  *chiploc = chipramvalue;
	  printf("writing chipram value of %4x to location %8x\n",chipramvalue,chiploc);
	  chiploc++;
	  if (chiploc > (unsigned short *) TIMBASE+TIMCHIP+4090)
	    {
		printf("load_waveformb: CHIP RAM index computation error.\n");
		printf("load_waveformb: aborting without fully programming timing module.\n\n");
		return;
	    }
      }
    /* Write terminal value (twice, as required by timing module) */
    *chiploc = 0x0000;
printf("wrote chipram terminal value of 0000 to location %8x\n",chiploc);
    chiploc++;
    *chiploc = 0x0000;
printf("wrote chipram terminal value of 0000 to location %8x\n",chiploc);

    /* GENERATE BLANKING VALUES */
    if (*total_pcp == 1)
      {
	  blankingvalue = blank_chip[0] & 0x001f;
	  if (blank_chip[0] & 0x0020)
	    blankingvalue=blankingvalue|0x0C00; /*shift pk*/
	  *blankloc = blankingvalue;
	  printf("writing blanking value of %4x to location %8x\n",blankingvalue,blankloc);
	  blankloc++;
      }
    else
      {
	  for (i=0; i<(*total_pcp)-1; i++)  /* step through blanking values */
	    {
		/* get lower 5 blanking bits and set STOPCT* bit */
		blankingvalue = 0x8000 | (blank_chip[i] & 0x001f);
		if (blank_chip[i] & 0x0020)
		  blankingvalue=blankingvalue|0x0C00; /*shift pk*/
		*blankloc = blankingvalue;
		printf("writing blanking value of %4x to location %8x\n",blankingvalue,blankloc);
		blankloc++;
	    }
	  i++;
	  blankingvalue = blank_chip[i] & 0x001f; /* load lower five bits, with
						     STOPCT* = 0; */
	  if (blank_chip[i] & 0x0020)
	    blankingvalue=blankingvalue|0xC00;    /* shift PK bit */
	  *blankloc = blankingvalue;
	  printf("writing blanking value of %4x to location %8x\n",blankingvalue,blankloc);
      }
}
