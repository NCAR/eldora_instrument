 /*
 *	$Id$
 *
 *	Module: DSPGO		 
 *	Original Author: Grant Gray 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:	 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.3  1994/08/24  16:28:18  eric
 * added "silent" mode to suppress diagnostic
 * printf's.
 *
 * Revision 1.2  1993/06/07  17:36:09  eric
 * Unset DMA and AUTO bits in PCRL. This improved operation
 * of PDF flag greatly!!
 *
 * Revision 1.1  1992/11/09  22:58:20  eric
 * Initial revision
 *
 *
 *
 * description:
 *
 *  Starts a DSP32C.
 *  arg1 = freq;  i.e. f1, f2, or f3
 *  arg2 = board;
 *  arg3 = DSP on board;
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

 
#include "stdioLib.h"
#include "ctype.h"
#include "stdarg.h"
#include "ELDRP7.h"

extern int Silent; 
unsigned long getaddr();
unsigned int  load8();
int dspgo(int rpt)

{
  va_list ap;
  unsigned long  bd, frq;
  unsigned char *pio, scr, *temp;
  int i, dsp, dspst, dspen, n, na, status;

  va_start(ap,rpt);

  n = rpt;
 
  for(na = 0; na < n; na++)
    {
    frq = va_arg(ap, int);              /* frequency 1, 2, or 3 */
    frq = (frq << 24) + EXTD_BASE;       /* frequency # * 10**6 + EXTD_BASE */
    bd = va_arg(ap, int);               /* Board number */
    bd = bd << 20;                      /* Board # * 10**5 = base addr */
    bd = bd + frq;                      /* Full base addr. of board */
    dsp = va_arg(ap, int);
    if(!Silent)
      printf("bd=%x dsp=%x\n", bd, dsp);
 
    if(dsp < 0)
         {
         dspst = 0;
         dspen = 3;
         }
    else
         {
         dspst = dsp;
         dspen = dsp;
         }
 
    for(i=dspst;i<=dspen;i++)
    {
         pio = (unsigned char *)(bd + (DSPSEL + ((unsigned long)i * DSPOFF)));
         /* Stop DSP */
         *(pio + PCRL) = DSPRESET;
 
         /* Check PIR and PDR */

         if(*(pio + PCRL) & PDF)scr = *(pio + PDRH);
	    /* If data flag set, rea */
         if(*(pio + PCRL) & PIF)scr = *(pio + PIRH); /*was PIRH */
   /* If int flag set, read  */
         /* Restart DSP */
         *(pio + PCRL) = 0;
	 *(pio + PCRL) = RUN;
	 *(pio + PCRL) = REGMAP + RUN + ENI;
	 temp = (unsigned char *)(pio + PCRL);
	 status = *temp & 0x1f;
	 if(status != REGMAP + RUN + ENI)
	   if(!Silent)
	     printf("Board %x, Processor %d Failed to Start; status = %x \n", bd,i,status);

    }
  }
    va_end(ap);
}









