/*
 *	$Id$
 *
 *	Module:	fill_nav_info	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/09/01  16:51:08  craig
 * Initial revision
 *
 * Revision 1.1  1992/08/25  20:43:04  craig
 * Initial revision
 *
 * description: This module fills the nav info block pointed to by
 *              the variable current_nav_pntr.  If the parameter sent
 *              to it is non-zero (true) this routine will also fill
 *              in the ascii at the start of the block.
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

void fill_nav_info(int ascii)
{

/* Define some general purpose variables */
char hour,min,sec,day,yr,mon;
short msec,jday;

/* If ascii is true write the ascii out to the current nav area */

if(ascii)
  {
      current_nav_pntr->s_nav_info.nav_info_id[0] = 'N';
      current_nav_pntr->s_nav_info.nav_info_id[1] = 'A';
      current_nav_pntr->s_nav_info.nav_info_id[2] = 'V';
      current_nav_pntr->s_nav_info.nav_info_id[3] = 'D';
      current_nav_pntr->s_nav_info.nav_info_len = sizeof(struct nav_info);
  }

/* Get the current time from the time of day board */
get_time(&hour,&min,&sec,&msec,&jday,&mon,&day,&yr);

current_nav_pntr->s_nav_info.julian_day = jday;
current_nav_pntr->s_nav_info.hours = hour;
current_nav_pntr->s_nav_info.minutes = min;
current_nav_pntr->s_nav_info.seconds = sec;

return;
}
