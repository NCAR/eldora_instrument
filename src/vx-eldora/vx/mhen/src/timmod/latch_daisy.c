/*
 *	$Id$
 *
 *	Module:	LATCH DAISY	 
 *	Original Author: Eric Loew
 *      Copyright National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  2002/03/12  18:33:38  eric
 * Initial revision
 *
 *
 * Initial revision
 *
 *
 description:
 
 Purpose:       Program ELDORA Timing Module and generate chips to latch
                the daisy chain value on a given Pulse Pair Processor.
                To be used for diagnostics only!
                (Does not load gate generators).
 Arguments:     num_chip       chip to program.
 Date Written:  05/05/95 
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "ELDRP7.h"
#define NOM_WIDTH 60
#define NOM_OFFSET 0
#define TOTAL_PCP 1
#define RPT_SEQ 1

int latch_daisy(short num_chip)

{
    unsigned short *blankloc,*nloc,*chiploc,chip_offset[6];
    unsigned short pkstart,f0start,f1start,f2start,f3start,f4start;
    unsigned short pkend,f0end,f1end,f2end,f3end,f4end;
    unsigned short chipramvalue,blankingvalue,mask;
    unsigned short chipindex,maxindex,i,chip_width[6];

    void timonb(void);
    void timoffb(void);
    
    nloc        = (unsigned short *)(TIMBASE + TIMPCPN);
    blankloc    = (unsigned short *)(TIMBASE + TIMBLANK);
    chiploc     = (unsigned short *)(TIMBASE + TIMCHIP);
    
    /* load N for desired PCP */
    
    *nloc = (unsigned short) (RPT_SEQ * 15000 / TOTAL_PCP)-1;
    
    /* ======================DO CHIPS======================= */

    for(i=0;i<6;i++)
        {
            chip_width[i] = NOM_WIDTH;
            chip_offset[i] = NOM_OFFSET;
        }
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
	  return(-1);
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
/*	  printf("writing chipram value of %4x to location %8x\n",chipramvalue,chiploc);
*/
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
/* printf("wrote chipram terminal value of 0000 to location %8x\n",chiploc); */
    chiploc++;
    *chiploc = 0x0000;
/* printf("wrote chipram terminal value of 0000 to location %8x\n",chiploc); */

    /* GENERATE BLANKING VALUES */

    switch(num_chip)
      {
        case 1:
            mask=0x0001;
            break;
        case 2:
            mask=0x0002;
            break;
        case 3:
            mask=0x0004;
            break;
        case 4:
            mask=0x0008;
            break;
        case 5:
            mask=0x0010;
            break;
      }
    blankingvalue =  mask & 0x001f;
    if (mask & 0x0020)
      blankingvalue=blankingvalue|0x0C00; /*shift pk*/
    *blankloc = blankingvalue;
/*    printf("writing blanking value of %4x to location %8x\n",blankingvalue,blankloc); */
    blankloc++;

    timonb();
    timoffb();
}



