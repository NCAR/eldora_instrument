/*
 *	$Id$
 *
 *	Module:	start_iru	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/09/28  23:03:57  craig
 * Initial revision
 *
 * description: This module set variable concerned with the iru interface to
 *              the valuse they should be at after receiving a start command. 
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

#include "hskpDef.h"
#include "hskpInt.h"
#include "hskpGbl.h"
#include "tp41vAdr.h"
#include "vmevmeDef.h"
#include "vme_hndshk.h"
#include "vmevmeAdr.h"
#include "vmevmeFunc.h"
#include "vmevmeGbl.h"
#include "iruDef.h"
#include "iruFunc.h"
#include "iruGbl.h"

void start_iru()
{
long i;

in_iru_isr = 0;
iru_isr_count = 0;
old_iru_interrupts = iru_rpntr->num_interrupts;


/***** Zero Indexes into all of the data arrays */

msec_longitude_indx = 0;
status_indx = 0;
latitude_indx = 0;
longitude_indx = 0;
wind_speed_indx = 0;
wind_direction_indx = 0;
ns_velocity_indx = 0;
ew_velocity_indx = 0;
heading_indx = 0;
drift_indx = 0;
altitude_indx = 0;
inertial_vspeed_indx = 0;
pitch_indx = 0;
roll_indx = 0;
integ_vert_acc_indx = 0;
vert_acc_indx = 0;
track_rate_indx = 0;
pitch_rate_indx = 0;

}


