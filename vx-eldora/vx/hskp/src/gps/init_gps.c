/*
 *	$Id$
 *
 *	Module: gpsInit.c		 
 *	Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/08/19  17:23:56  craig
 * Initial revision
 *
 *
 * description:  Initializes the gps handling pointers etc.
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
#include "gpsGbl.h"
#include "gpsFunc.h"
#include "minDef.h"
#include "minFunc.h"
#include "tp41vAdr.h"
#include "vmevmeDef.h"
#include "vmevmeAdr.h"
#include "vmevmeFunc.h"
#include "vmevmeGbl.h"

/* rpc, status, commands and logfile includes */
#include "HskpStatus.h"

void init_gps(short mbox)
{
/* Set the pointers to the gps data areas */

gps_status = (char *)GPS_STATUS + STANDARD_BASE + GPS_BASE;
gps_command = (short *)GPS_COMMAND + STANDARD_BASE + GPS_BASE;
gps_mbox = (short *)GPS_MBOX + STANDARD_BASE + GPS_BASE;
gps_hndshk = (short *)GPS_HNDSHK + STANDARD_BASE + GPS_BASE;
gps_data_loc = (long *)GPS_DATA_LOC + STANDARD_BASE + GPS_BASE;

/* Command interface to use the mailbox interrupt (mbox!=0) or not (mbox==0) */

*gps_mbox = mbox;

/* Set the gps status byte to indicate no errors */

currStatus->gps = 0;

/* Setup the TP41V to handle mailbox interrupts */

wcio(0,"a",0xc6);  /* Writes correct register on the TP41V to allow
                      dual porting the memory */

/* Dual port the top 2 MBytes of DRAM to VME addresses 0x200000 thru
   0x3FFFFF, also creates mailbox interrupts in top 16 KBytes */

creatStdMailbox((long)0x40200000,(long)0x200000);

/* Connect the interrupt handler to the first mailbox interrupt */

connectMailbox((VOIDFUNCPTR)gps_isr,0,0);

/* Create pointer to the mailbox address the gps card will use and clear
   that location */

tp41_mbox_0 = (char *)(DRAM_BASE + TP41_MBOX_0);
*tp41_mbox_0 = 0;

return;
}
