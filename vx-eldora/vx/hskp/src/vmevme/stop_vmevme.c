/*
 *	$Id$
 *
 *	Module:	stop_vmevme	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1992/09/03  20:26:30  craig
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/25  20:43:01  craig
 * Initial revision
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

void stop_vmevme()
{

/* Clear the "Hi Radar" in the radar processors to get them  to stop */

fore_vmehndshk->salute[0] = ' ';
fore_vmehndshk->salute[1] = ' ';
fore_vmehndshk->salute[2] = ' ';
fore_vmehndshk->salute[3] = ' ';
fore_vmehndshk->salute[4] = ' ';
fore_vmehndshk->salute[5] = ' ';
fore_vmehndshk->salute[6] = ' ';
fore_vmehndshk->salute[7] = ' ';

aft_vmehndshk->salute[0] = ' ';
aft_vmehndshk->salute[1] = ' ';
aft_vmehndshk->salute[2] = ' ';
aft_vmehndshk->salute[3] = ' ';
aft_vmehndshk->salute[4] = ' ';
aft_vmehndshk->salute[5] = ' ';
aft_vmehndshk->salute[6] = ' ';
aft_vmehndshk->salute[7] = ' ';

}
