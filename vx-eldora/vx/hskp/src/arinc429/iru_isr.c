/*
 *	$Id$
 *
 *	Module: iru_isr.c		 
 *	Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/09/28  23:03:51  craig
 * Initial revision
 *
 * Revision 1.1  1992/09/01  15:56:56  craig
 * Initial revision
 *
 *
 * description:  This routine handles an interrupt from the IRU interface.
 *               Also, it will handle being called if the polled handshake
 *               method of using the IRU interface is enabled.
 *        
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define scope extern

/* Include fifty million vx-works .h files */

#include "vxWorks.h"
#include "math.h"
#include "stdioLib.h"
#include "intLib.h"
#include "memLib.h"
#include "semLib.h"
#include "taskLib.h"
#include "tyLib.h"
#include "ioLib.h"
#include "in.h"
#include "systime.h"
#include "sysLib.h"


/* include fifty million .h files to deal with the header formats */
#include "Volume.h"
#include "Waveform.h"
#include "RadarDesc.h"
#include "FieldRadar.h"
#include "CellSpacing.h"
#include "Parameter.h"
#include "NavDesc.h"
#include "InSitu.h"
#include "Ray.h"
#include "Platform.h"
#include "FieldParam.h"
#include "IndFreq.h"
#include "TimeSeries.h"
#include "NavInfo.h"
#include "Ins.h"
#include "MiniRIMS.h"
#include "Gps.h"
#include "InSituData.h"

#include "Header.h"
extern HeaderPtr inHeader;

/* include the .h files that are housekeeper code specific */

#include "hskpDef.h"
#include "hskpInt.h"
#include "hskpGbl.h"
#include "tp41vAdr.h"
#include "vmevmeDef.h"
#include "vme_hndshk.h"
#include "vmevmeAdr.h"
#include "vmevmeFunc.h"
#include "vmevmeGbl.h"
#include "iruDef.h"
#include "iruFunc.h"
#include "iruGbl.h"

/* rpc, status, commands and logfile includes */
#include "HskpStatus.h"

void iru_isr(void)
{
struct ins_data *where_data_goes;
long i, *long_pntr;
int num_words, test;

/* Perform the necessary tasks to clear the interrupt */

/* Set the global flag */
in_iru_isr = 1;
iru_isr_count++;

/* Determine if this routine was called due to an error (interface
received 300 ARINC words before detecting a latitude label) if error,
give indication */

if((iru_rpntr->num_interrupts % 2) == 0)   /* Even numbered Interrupt? */
  {
      long_pntr = (long *)&iru_rpntr->buf1[0];
      num_words = iru_rpntr->words_buf1;
  }
else   /* It is an odd numbered interrupt */
  {
      long_pntr = (long *)&iru_rpntr->buf2[0];
      num_words = iru_rpntr->words_buf2;
  }

if(num_words == 300)  /* never found longitude */
  currStatus->iru |= (char)IRU_WORDS_BAD;
else currStatus->iru &= (char)~IRU_WORDS_BAD;

/* Save the current iru interrupt counter */
old_iru_interrupts = iru_rpntr->num_interrupts;

/****************************************************/
/* Process the data buffer regardless of its status */
/****************************************************/

/* Convert the current INS data block */

where_data_goes = (ins_data *)(&current_nav_pntr->
                             s_ins_data.ins_data_id[0]);

/* printf("In iru_isr calling convert iru, num words = %d source = %x sink = %x\n"
       ,num_words,long_pntr,where_data_goes); */
test = convert_iru(num_words,long_pntr,where_data_goes);

/* If the INS data block is completely full, then go ahead and handle sending
   the navigation record out test, will be true if the buffer is full */

if(test)
  {
      if(nav_flag & IRU_ON == 0)  /* Did we complete two buffers before
                                          the GPS or MiniRIMS completed one? */
	{
	    /* Set the IRU bit in nav_flag */

	    nav_flag = nav_flag | IRU_ON;

	    /* Check to see if all the bits are set (meaning a complete
	       seconds worth of navigation data is ready to record) */

	    if((nav_flag & nav_mask) == nav_mask)
	      {
		  currStatus->fore &= (char)(~IRU_SYNC);
		  nav_current_size += sizeof(struct nav_data);
		  nav_flag = 0;

		  /* Check to see if current buffer is full */

		  if(nav_current_size > MAX_NAV_SIZE)
		    {

			/* Current buffer is full, send it to record */

			fore_vmehndshk->nav_hndshk[nav_current_index] = 1;
			nav_current_size = sizeof(struct nav_data);

			++nav_current_index;
			if(nav_current_index > 1) /* Been filling area #1? */
			  {
			      /* Now fill area #0 */
			      nav_current_index = 0;
			      current_nav_pntr = (struct nav_data *)
				(FIRST_NAV_OFFSET + STANDARD_BASE
				 + FORE_STAND_START);
			  }
			else /* We just filled area #0 */
			  current_nav_pntr = (struct nav_data *)
			    (SEC_NAV_OFFSET + STANDARD_BASE
			     + FORE_STAND_START);

			/* Check to see that the last buffer was recorded */

			if(fore_vmehndshk->nav_hndshk[nav_current_index] !=
                          (short)0xAFFF) 
			  currStatus->fore = currStatus->fore | (char)NAV_FULL;
			else
			  currStatus->fore = currStatus->fore &
			    (char)(~NAV_FULL);

		    } /* If test on full navigation data buffer */

		  else  /* There is room in the present buffer for more
			   nav data records */
		    current_nav_pntr++;
	      } /* If test on all bits set in nav_flag */

	} /* Check to see if the IRU bit was already set */

	    else /* We got two seconds worth of IRU data before getting
		    one seconds worth of either GPS or MiniRIMS data */
	      currStatus->fore = currStatus->fore | IRU_SYNC;

  } /* If test on variable test to see if IRU buffer is complete */
return;
}
