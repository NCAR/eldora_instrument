/*
 *	$Id$
 *
 *	Module:	update_testpulse	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/08/25  20:43:04  craig
 * Initial revision
 *
 * description: This module updates the ongoing test pulse calibration.
 *              This routine is called when testpulse_dwell_count is
 *              greater than testpulse_max_count.
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

void update_testpulse()
{
/* Define some general purpose variables */
unsigned char B, nr;
unsigned long T;
double frequency, temp;
unsigned char unitnum, ecbaddr,test,attenuation, foraft;
int timeout;

/* Zero the testpulse dwell counter */
tp_dwell_count = 0;

/* Check to see if I am in control of the fore test pulse, or if the
   fore radar processor is in control */

if(fore_vmehndshk->tpulse_flg != 3)
  {
      /* I am in charge of the test pulse */

     fore_vmehndshk->tpulse_freq_num = tp_freq_count + 1;
     frequency = fore_half_nyquist[tp_freq_count][tp_freq_offset_count];
     attenuation = tp_atten;
     fore_vmehndshk->tpulse_flg = 1;
 }
else   /* The fore radar processor is in charge of the test pulse */
  {
      frequency = fore_vmehndshk->tpulse_freq * 1.e9;
      attenuation = fore_vmehndshk->tpulse_atten;
      fore_vmehndshk->tpulse_flg = 4;
  }

/* Set the fore test pulse frequency */

ecbaddr = ECBRFFOR;
B = (unsigned char)(361 - (int)(frequency / 30.0e6));
temp = 15445.3333333333333 - (frequency / 703125.0);
temp = (16777216.0 / (B+1)) * temp;
T = (unsigned long)(temp + 0.5);
unitnum = 6;
nr = 1;
timeout = 0;
do
  {
      timeout++;
  }while((test = ecbSetDDS(ecbaddr,unitnum,B,T,nr) != 0) &&
	 timeout < 3000);

/* Set the fore testpulse attenuation level */
ecbaddr = ECBATTEN;
foraft = 1;
timeout=0;
do
  {
      timeout++;
  }while((test = ecbSetAtten(ecbaddr, foraft, attenuation) != 0) &&
	  timeout < 3000);  



/* Check to see if I am in control of the aft test pulse, or if the
   aft radar processor is in control */

if(aft_vmehndshk->tpulse_flg != 3)
  {
      /* I am in charge of the test pulse */

     aft_vmehndshk->tpulse_freq_num = tp_freq_count + 1;
     frequency = aft_half_nyquist[tp_freq_count][tp_freq_offset_count];
     attenuation = tp_atten;
     aft_vmehndshk->tpulse_flg = 1;

     /* Update the counters for next time */

     tp_freq_count++;
     if((tp_freq_count + 1) >= fraddes->num_freq_trans)
       {
	   tp_freq_count = 0;
	   tp_atten += TP_ATTEN_STEP;
	   if(tp_atten > TP_ATTEN_MAX)
	     {
		 tp_atten_start++;
		 if(tp_atten_start >= TP_ATTEN_STEP)
		   tp_atten_start = 0;
		 tp_atten = tp_atten_start;

		 tp_freq_offset_count++;
		 if(tp_freq_offset_count > 1)
		   tp_freq_offset_count = 0;

	     }
       }
     
 }

else   /* The aft radar processor is in charge of the test pulse */
  {
      frequency = aft_vmehndshk->tpulse_freq * 1.e9;
      attenuation = aft_vmehndshk->tpulse_atten;
      aft_vmehndshk->tpulse_flg = 4;
  }

/* Set the aft test pulse frequency */

ecbaddr = ECBRFAFT;
B = (unsigned char)(361 - (int)(frequency / 30.0e6));
temp = 15445.3333333333333 - (frequency / 703125.0);
temp = (16777216.0 / (B+1)) * temp;
T = (unsigned long)(temp + 0.5);
unitnum = 6;
nr = 1;
timeout = 0;
do
  {
      timeout++;
  }while((test = ecbSetDDS(ecbaddr,unitnum,B,T,nr) != 0) &&
	 timeout < 3000);

/* Set the aft testpulse attenuation level */
ecbaddr = ECBATTEN;
foraft = 0;
timeout = 0;
do
  {
      timeout++;
  }while((test = ecbSetAtten(ecbaddr, foraft, attenuation) != 0) &&
	 timeout < 3000);  

}
