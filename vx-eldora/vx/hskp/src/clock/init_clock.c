/*
 *	$Id$
 *
 *	Module:	init_clock.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/08/14  21:42:52  reif
 * Initial revision
 *
 *
 * description: The purpose of this module is to initialize all of the
 *              global variables used with the time-of-day (tod)
 *              board.  It also sets up the sub-second interrupt
 *              but does not turn it on and then 
 *              corrects the julian day array for leap year
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
#include "gpsFunc.h"
#include "minDef.h"
#include "minFunc.h"
#include "tp41vAdr.h"
#include "vmevmeDef.h"
#include "vmevmeAdr.h"
#include "vmevmeFunc.h"
#include "vmevmeGbl.h"

void init_clock(short div_525) 
{
unsigned char vyr;
long i;

/* INITIALIZE ALL OF THE Clock chip related GLOBAL POINTERS *****************/

hsec = (unsigned char *)(HUNDRETHS);
sec  = (unsigned char *)(SECONDS);
min =  (unsigned char *)(MINUTES);
hr  =  (unsigned char *)(HOURS);
day =  (unsigned char *)(DAY);
mon =  (unsigned char *)(MONTH);
yr  =  (unsigned char *)(YEAR);
tod_cmnd = (unsigned char *)(TOD_CMND_REG);
int_msk = (unsigned char *)(INT_STAT_AND_MASK);

/* Initialized the IRIG-B related pointers */

irigb = (unsigned char *)(ENIRIGB);
strg  = (unsigned char *)(SELSTRG);
isec = (unsigned char *)(SELSEC);
imin = (unsigned char *)(SELMIN);
ihr  = (unsigned char *)(SELHRS);
id1  = (unsigned char *)(SELDAY1);
id2  = (unsigned char *)(SELDAY2);

/* Pointers to the sub-second interrupt handling stuff */

msec = (unsigned short *)(READ_MS);
sel_525 = (short *)SEL_525;
reset_525 = (short *)RST_525;

/* Pointers to the BIM registers */

tod_bim_cr0 = (unsigned char *)BIM_CR0;
tod_bim_cr1 = (unsigned char *)BIM_CR1;
tod_bim_cr2 = (unsigned char *)BIM_CR2;
tod_bim_cr3 = (unsigned char *)BIM_CR3;
tod_bim_vr0 = (unsigned char *)BIM_VR0;
tod_bim_vr1 = (unsigned char *)BIM_VR1;
tod_bim_vr2 = (unsigned char *)BIM_VR2;
tod_bim_vr3 = (unsigned char *)BIM_VR3;


/* Set the one second interrupt up so the sub-second counter is cleared
   Each second */

*int_msk=0x08;      /* Enable 1 second interrupts for sub second counters */
*sel_525 = div_525; /* This sets the sub second interrupt period */

/* Now program the BIM to interrupt on both the 1 second interrupt
   from the clock and the sub second interrupt from the 525 */

/* Check for leap year, update the jday_calc array if it is */

vyr = *hsec;  /* To latch the time and date registers */
vyr = *yr;    /* read the read */

if((((int)(vyr/4))*4 == vyr) && (jday_calc[2] == 60))
  {
      for(i=2; i<12; i++)
	jday_calc[i]++;
  }

return;
}
