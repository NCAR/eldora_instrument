/*
 *	$Id$
 *
 *	Module:	sync.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/08/30  20:12:57  craig
 * Initial revision
 *
 * Revision 1.1  1992/08/14  21:42:52  reif
 * Initial revision
 *
 *
 * description: The purpose of this module is to sync the time-of-day (tod)
 *              board's clock chip to the incomming IRIG-B signal coming
 *              from ADS (or any other source).
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

char sync_irig() 
{

unsigned char hseconds; 
short timeout;

/********** ENABLE LOADING OF IRIG REGISTER **************/

*irigb=0x00; /* Enable IRIGB data into registers */

/* Wait for last IRIG-B register to be loaded */

timeout = 0;
do{
    timeout++;
    taskDelay(2);
    }while(((*strg & LDDAY2) != LDDAY2) && (timeout < 90));
if(timeout >= 90) return((char)0);
taskDelay(2);

/************** LOAD CLOCK REGISTERS WITH IRIG DATA ****************/

*tod_cmnd=0x04; /* Stop clock for loading,leave in 24 hr mode */
*hsec=0x00;     /* Init hundreths to zero */
*sec=3+((unsigned char)0x0F&*isec)+((*isec>>4)*10); /* ADD 1 second */

*min=((unsigned char)0x0F&*imin)+((*imin>>4)*10); /* Load minute register
						     with IRIG minutes */

*hr=((unsigned char)0x0F&*ihr)+((*ihr>>4)*10); /* Load hours register with
						  IRIG hours */

/*********** WAIT FOR TIME MARK BEFORE STARTING CLOCK ********/

*irigb=0x00;
timeout = 0;
do{
      timeout++;
      taskDelay(2);
  }while(((*strg & TMRKFND) != TMRKFND) && (timeout < 90));


/********* START CLOCK (Whether we were successful or not *******************/

*tod_cmnd=0x1c; /* Normal Mode, Ints enabled, 12 hr mode, 32.768kHz crystal */
if(timeout >= 90) return((char)0);

return((char)1);
}
