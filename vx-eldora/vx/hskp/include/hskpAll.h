/* Include this file at the start of any houskeeping function */

#define OK_RPC

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
#include "HeaderRpc.h"
#define inHeader Hdr

/* include the .h files that are housekeeper code specific */
#include "cntrlDef.h"
#include "cntrlFunc.h"
#include "cntrlGbl.h"
#include "ecbAdr.h"
#include "ecbErrBound.h"
#include "ecbFunc.h"
#include "ecbMaster.h"
#include "ecbSem.h"
#include "ecbStat.h"
#include "gpsDef.h"
#include "gpsFunc.h"
#include "gpsGbl.h"
#include "hskpDef.h"
#include "hskpFunc.h"
#include "hskpGbl.h"
#include "hskpInt.h"
#include "iruDef.h"
#include "iruFunc.h"
#include "iruGbl.h"
/* #include "minDef.h" */
/* #include "minFunc.h" */
#include "pwrDef.h"
#include "pwrFunc.h"
#include "todDef.h"
#include "todFunc.h"
#include "todGbl.h"
#include "tp41vAdr.h"
#include "vme_hndshk.h"
#include "vmevmeAdr.h"
#include "vmevmeDef.h"
#include "vmevmeFunc.h"
#include "vmevmeGbl.h"
#include "HskpCmd.h"
#include "HskpStatus.h"


