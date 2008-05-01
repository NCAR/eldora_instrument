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
 * Revision 1.5  1994/05/20  20:36:53  craig
 * *** empty log message ***
 *
 * Revision 1.1  1992/09/01  15:56:56  craig
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
#include "hskpAll.h"
#include "tp41.h"

void gps_isr(void)
{
char *where_data_is, *where_data_goes, *last_gps_pntr;
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

/* Set the global flag */
in_gps_isr = 1;

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
      
      /* Update the latest and greatest GPS data area */

      last_gps_pntr = (char *)&last_gps_data.gps_data_id[0];
      for(i=0; i<data_size; i++)
	*last_gps_pntr++ = *where_data_is++;

  }       /* A good data buffer is ready */
return;
}
