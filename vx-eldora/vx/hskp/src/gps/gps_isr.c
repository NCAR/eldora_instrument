/*
 *	$Id$
 *
 *	Module: gps_isr.c		 
 *	Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/09/01  15:56:56  craig
 * Initial revision
 *
 * Revision 1.1  1992/08/19  17:23:56  craig
 * Initial revision
 *
 *
 * description:  This routine handles an interrupt from the GPS interface.
 *               Also, it will handle being called if the polled handshake
 *               method of using the GPS interface is enabled.
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

#define TP41LIB
#include "tp41Lib.h"

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

#include "cntrlDef.h"
#include "cntrlFunc.h"
#include "hskpDef.h"
#include "hskpInt.h"
#include "hskpGbl.h"
#include "todDef.h"
#include "todFunc.h"
#include "todGbl.h"
#include "ecbAdr.h"
#include "ecbErrBound.h"
#include "ecbFunc.h"
#include "ecbMaster.h"
#include "ecbSem.h"
#include "ecbStat.h"
#include "pwrDef.h"
#include "pwrGbl.h"
#include "pwrFunc.h"
#include "gpsDef.h"
#include "gpsGbl.h"
#include "gpsFunc.h"
#include "minDef.h"
#include "minFunc.h"
#include "tp41vAdr.h"
#include "vmevmeDef.h"
#include "vmevmeAdr.h"
#include "vmevmeFunc.h"
#include "vmevmeGbl.h"

/* rpc, status, commands and logfile includes */
#include "HskpStatus.h"

void gps_isr(void)
{
char *where_data_is, *where_data_goes;
long i, data_size;
union
  {
    short words[2];
    long add;
  }address;

/* Perform the necessary tasks to clear the interrupt */

*TP41_VME_MBOX_CLEAR = 0;
*TP41_CIO_2_CNTRL_ADRS = ZCIO_PBCS;
*TP41_CIO_2_CNTRL_ADRS = ZCIO_CS_CLIPIUS;
*tp41_mbox_0 = 0;

/* Determine if this routine was called due to an error (*gps_hndshk = 0x81)
   or due to a data (= 0x80), if error, give indication and return */

if(*gps_hndshk == 0x81)   /* Were we alerted due to a GPS error? */
  {
      currStatus->gps = *gps_status;

      /* Clear the handshake word */

      *gps_hndshk = (short)0;
  }
else  /* We have a good data buffer ready */
  {
      currStatus->gps = (char)0;
      *gps_hndshk = (short)0;

      address.words[0] = *gps_d_loc_h;
      address.words[1] = *gps_d_loc_l;

      where_data_is = (char *)(address.add + STANDARD_BASE + GPS_BASE);
      data_size = sizeof(struct gps_data);

      /* See if GPS data has already been inserted in this second's data
         record */

      if((GPS_ON & nav_flag) == 0)  /* If true no GPS data has been inserted
                                       this second */
	{
	    /* Copy over the current GPS data block */

	    where_data_goes = (char *)(&current_nav_pntr->
                                          s_gps_data.gps_data_id[0]);
	    for(i=0; i<data_size; i++)
	      *where_data_goes++ = *where_data_is++;

	    /* Set the GPS bit in nav_flag */
	    nav_flag = nav_flag | GPS_ON;
	}
      else    /* The GPS bit is set in nav_flag */
	{

	    /* Check to see if all the bits are set (meaning a complete
               seconds worth of navigation data is ready to record) */

	    if((nav_flag & nav_mask) == nav_mask)
	      {
		  nav_current_size += sizeof(struct nav_data);

		  /* Check to see if current buffer is full */

		  if(nav_current_size > MAX_NAV_SIZE)
		    {

			/* Current buffer is full, send it to record */

			fore_vmehndshk->nav_hndshk[nav_current_index] = 1;
		        nav_current_size = sizeof(struct nav_data);

			if(++nav_current_index > 1) /* Been filling area #1? */
			  {
			      /* Now fill area #0 */
                              nav_current_index = 0;
			      current_nav_pntr = (struct nav_data *)
				(FIRST_NAV_OFFSET + STANDARD_BASE + 
				 FORE_STAND_START);
			  }
			else /* We just filled area #0 */
			  current_nav_pntr = (struct nav_data *)
				(SEC_NAV_OFFSET + STANDARD_BASE + 
				 FORE_STAND_START);

			/* Check to see that the last buffer was recorded */

			if(fore_vmehndshk->nav_hndshk[nav_current_index] !=
                           (short)0xAFFF) 
			  currStatus->fore = currStatus->fore | (char)NAV_FULL;
			else
			  currStatus->fore = currStatus->fore
                                                        & (char)(~NAV_FULL);
		    }
		  else  /* There is room in the present buffer for more
                           nav data records */
			current_nav_pntr++;

		  /* Now we have a new place to put the GPS data
	             so copy over the current GPS data block */

		  where_data_goes = (char *)(&current_nav_pntr->
                                             s_gps_data.gps_data_id[0]);
		  for(i=0; i<data_size; i++)
		    *where_data_goes++ = *where_data_is++;

		  /* Set only GPS bit in nav_flag */
		  nav_flag = GPS_ON;
	      }
	    else /* We got two seconds worth of GPS data before getting
		    one seconds worth of either IRU or ARINC 429 data */
	      currStatus->fore = currStatus->fore | GPS_SYNC;
	} /* The GPS bit is set in gps_flg */
  }       /* A good data buffer is ready */
return;
}
