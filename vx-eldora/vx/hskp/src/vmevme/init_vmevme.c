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
 *
 * description: This module initializes the vme to vme interfaces and their
 *              handshake areas 
 *              
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define SCOPE extern

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
#include "pwrGbl.h"
#include "pwrDef.h"
#include "pwrFunc.h"
#include "gpsDef.h"
#include "gpsFunc.h"
#include "minDef.h"
#include "minFunc.h"
#include "tp41vAdr.h"
#include "vmevmeAdr.h"
#include "vmevmeDef.h"
#include "vmevmeGbl.h"
#include "vmevmeFunc.h"

init_vmevme()
{
/* Define some general purpose variables */

fore_vmehndshk = (struct vmevmehndshk *)(FORE_STAND_START + STANDARD_BASE); 
aft_vmehndshk = (struct vmevmehndshk *)(AFT_STAND_START + STANDARD_BASE); 

fore_local_status = (char *)(LOCAL_STATUS_REG + FORE_SHORT_START + SHORT_BASE);
fore_local_command = (char *)(LOCAL_STATUS_REG + FORE_SHORT_START + SHORT_BASE);

}
