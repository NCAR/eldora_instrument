/*
 *	$Id$
 *
 *	Module:	start_vmevme	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/08/14  20:44:35  craig
 * Initial revision
 *
 *

/* Include fifty million vx-works .h files */

#include "vxWorks.h"
#include "math.h"
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#include "intLib.h"
#include "memLib.h"
#define SCOPE extern
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
#include "vme_hndshk.h"
#include "vmevmeAdr.h"
#include "vmevmeFunc.h"
#include "vmevmeGbl.h"

void start_vmevme()
{
/* Define some general purpose variables */
long i, iru_lags, msecs_today;
char hr,min,sec,mon,day,yr;
short msec,jday;
struct DATARAY *ray_pntr;

/* Set each of the handshake areas to the "good data broadcast" status word
   of 0xAFFF */

for(i=0; i<31; i++)
  fore_vmehndshk->radar_hndshk[i] = 0xAFFF;

for(i=0; i<31; i++)
  aft_vmehndshk->radar_hndshk[i] = 0xAFFF;

/* The Radar processors will be 4 dwell times behind so set each of the
 indecies to two (2) (starts at 0, increments once before it is used) */

current_index = 2;
ads_current_index = 0;

/* Set each of the indecies to zero */

current_offset = FIRST_RADAR_OFFSET + current_index * RADAR_SIZE_INCR;
/* Make an index that will follow behind the navigational data and make the
nav_current_index = 0;
   radar processors send it out over the mcpl.   
Begin by calculating the number of lags the iru data will lag behind */

iru_lags = 200/dwelltime_msec + 2;  /* +2 because this should lag
iru_lag_index = current_index - iru_lags;
nav_current_offset = FIRST_NAV_OFFSET;
   into all entries of msecs_ray */

get_time(&hr,&min,&sec,&msec,&jday,&mon,&day,&yr);
msecs_today = msec + 1000 * (sec + 60 * (min + 60 * hr));

radar_offset = FIRST_RADAR_OFFSET;
for(i=0; i<NUM_RADAR_HNDSHK; i++)
  {
      platform_status[i] = 0;
      msecs_ray[i] = msecs_today;
      /* Put things in each record that only change with a header */
      ray_pntr->this_rayi.peak_power = -999.0;
      ray_pntr->this_rayi.ray_status = 0;
      ray_pntr->this_plat.altitude_agl = -999.0;
      ray_pntr->this_plat.tilt = FORE_TILT_ANGLE;
      ray_pntr->this_fdata.data_sys_status = 0;

      ray_pntr = (struct DATARAY *)(radar_offset + STANDARD_BASE +
				    AFT_STAND_START);
      ray_pntr->this_rayi.peak_power = -999.0;
      ray_pntr->this_rayi.ray_status = 0;
      ray_pntr->this_plat.altitude_agl = -999.0;
      ray_pntr->this_plat.tilt = AFT_TILT_ANGLE;
      ray_pntr->this_fdata.data_sys_status = 0;

      radar_offset += RADAR_SIZE_INCR;
  }


/* Do things to handle recording of nav data */

nav_flag = 0;
nav_current_index = 0;
nav_current_size = sizeof(struct nav_data);

/* Put ASCII characters into both nav structures */

current_nav_pntr = (struct nav_data *)(SEC_NAV_OFFSET + 
                    STANDARD_BASE + FORE_STAND_START);
fill_nav_info((int)1);
current_nav_pntr = (struct nav_data *)(FIRST_NAV_OFFSET + 
                    STANDARD_BASE + FORE_STAND_START);
fill_nav_info((int)1);

i = (long)(MAX_NAV_SIZE / nav_current_size);
fore_vmehndshk->nav_length = (short)(nav_current_size * i);

/* Clear the polled handshake words */
fore_vmehndshk->polled = 0;
aft_vmehndshk->polled = 0;

/* Now write "Hi Radar" to the radars to get them going */


aft_vmehndshk->salute[0] = 'H';
aft_vmehndshk->salute[1] = 'I';
aft_vmehndshk->salute[2] = ' ';
aft_vmehndshk->salute[3] = 'R';
aft_vmehndshk->salute[4] = 'A';
aft_vmehndshk->salute[5] = 'D';
aft_vmehndshk->salute[6] = 'A';
aft_vmehndshk->salute[7] = 'R';

fore_vmehndshk->salute[0] = 'H';
fore_vmehndshk->salute[1] = 'I';
fore_vmehndshk->salute[2] = ' ';
fore_vmehndshk->salute[3] = 'R';
fore_vmehndshk->salute[4] = 'A';
fore_vmehndshk->salute[5] = 'D';
fore_vmehndshk->salute[6] = 'A';
fore_vmehndshk->salute[7] = 'R';

}
