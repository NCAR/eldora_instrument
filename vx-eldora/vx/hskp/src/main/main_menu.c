/*
 *	$Id$
 *
 *	Module:	main_menu	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/09/08  23:15:54  craig
 * Initial revision
 *
 * Revision 1.2  1992/09/08  22:55:48  craig
 * *** empty log message ***
 *
 * Revision 1.1  1992/09/04  15:49:48  craig
 * Initial revision
 *
 * Revision 1.4  1992/09/03  19:57:11  craig
 * *** empty log message ***
 *
 * Revision 1.3  1992/09/01  16:21:52  craig
 * *** empty log message ***
 *
 * Revision 1.2  1992/08/25  20:43:42  craig
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/19  17:27:16  craig
 * Initial revision
 *
 *
 * description: This module provides executive control of the ELDORA 
 *              Housekeeping processor by reading a header passed
 *              by the Control Processor and following command passed
 *              by the control processor. 
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

#include "HskpCmd.h"
#include "HskpStatus.h"
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

main_menu()
{
short cmd;

do{
    printf("Main Menu For The Housekeeper Executive Program\n\n");
    printf("0) End main menu\n");
    printf("1) Start the Executive program\n");
    printf("2) Stop the Executive program\n");
    printf("3) Reload the Executive program\n");
    scanf("%d",&cmd);
    getchar();
    switch(cmd)
      {
	case 1:         /* Start the executive */
	  stop_flag = 0;
	  break;
	case 2:        /* Stop the executive */
	  stop_flag = 1;
	  break;
	case 3:        /* Reload the executive */
	  reload_flag = 1;
	  break;
	default:
	  break;
      }

}while(cmd != 0);

}
