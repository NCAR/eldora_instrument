/*
 *	$Id$
 *
 *	Module:	init_vmevme	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1992/08/25  20:42:50  craig
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/14  20:44:35  craig
 * Initial revision
 *
 *
 * description: This module initializes the vme to vme interfaces and their
 *              handshake areas 
 *              
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define SCOPE extern

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

void init_vmevme()
{
/* Define some general purpose variables */
long i;


/* Setup all of the pointers to the VME to VME interface areas */

fore_vmehndshk = (struct vmevmehndshk *)(FORE_STAND_START + STANDARD_BASE); 
aft_vmehndshk = (struct vmevmehndshk *)(AFT_STAND_START + STANDARD_BASE); 

fore_local_status = (char *)(LOCAL_STATUS_REG + FORE_SHORT_START + SHORT_BASE);
fore_local_command = (char *)(LOCAL_COMMAND_REG +
			      FORE_SHORT_START + SHORT_BASE);
fore_remote_status = (char *)(REMOTE_STATUS_REG +
			      FORE_SHORT_START + SHORT_BASE);
fore_remote_command = (char *)(REMOTE_COMMAND_REG +
			       FORE_SHORT_START + SHORT_BASE);
aft_local_status = (char *)(LOCAL_STATUS_REG + AFT_SHORT_START + SHORT_BASE);
aft_local_command = (char *)(LOCAL_COMMAND_REG +
			      AFT_SHORT_START + SHORT_BASE);
aft_remote_status = (char *)(REMOTE_STATUS_REG +
			      AFT_SHORT_START + SHORT_BASE);
aft_remote_command = (char *)(REMOTE_COMMAND_REG +
			       AFT_SHORT_START + SHORT_BASE);

/* Clear the fore and aft test pulse handshake areas */

for(i=0; i<0x100000; i++)
  fore_vmehndshk->salute[i] = 0;
for(i=0; i<0x100000; i++)
  aft_vmehndshk->salute[i] = 0;

/* Clear the fore and aft sweep numbers to zero */
fore_sweep_num = 0;
aft_sweep_num = 0;

/* Create the navigation data mask */

nav_mask = GPS_ON + MINIRIMS_ON + IRU_ON + KALMAN_ON;

/* Attach the vmevme_isr to the proper interrupt vector */
if(intConnect((VOIDFUNCPTR *)(VME_VME_VEC * 4),
          (VOIDFUNCPTR)vmevme_isr,0) == ERROR)
  {
      fprintf(stderr,"failed to connect interrupt vector %d\n",VME_VME_VEC);
      exit(1);
  }

}
