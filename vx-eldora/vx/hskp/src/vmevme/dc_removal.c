/*
 *	$Id$
 *
 *	Module:	dc_removal	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description: This module handshakes with the radar processors over the
 *              vme to vme interfaces while they are doing DC Removal
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

void dc_removal()
{
    dc_remove_flag = 0;
    printf("Entering DC remove Handshake\n");
    printf("Will wait on radar processors to get ready\n");

    /* Wait for the radar processors to get ready */

    do{
	taskDelay(20);
    }while((fore_vmehndshk->start_hndshk == 0) ||
	   (aft_vmehndshk->start_hndshk == 0)); 

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

    printf("Got the radar processors started\n");
    do{
	aft_vmehndshk->dc_remove_hndshk = 0;
	fore_vmehndshk->dc_remove_hndshk = 0;

	do{

	    taskDelay(50);
	    printf("Waiting for both non zero fore= %d aft= %d\n",
		   fore_vmehndshk->dc_remove_hndshk,
		   aft_vmehndshk->dc_remove_hndshk);

	}while((fore_vmehndshk->dc_remove_hndshk == 0) ||
			   (aft_vmehndshk->dc_remove_hndshk == 0));

	printf("Found both non zero fore= %d aft= %d\n",
	       fore_vmehndshk->dc_remove_hndshk,
	       aft_vmehndshk->dc_remove_hndshk);

    }while((fore_vmehndshk->dc_remove_hndshk != 2) && 
	   (aft_vmehndshk->dc_remove_hndshk != 2));

    /* Check to see if something is funny,
       if so handshake the OK radar processor through its DC removal */

    if((fore_vmehndshk->dc_remove_hndshk == 2) && 
	   (aft_vmehndshk->dc_remove_hndshk != 2))
      {
	  do{
	      aft_vmehndshk->dc_remove_hndshk = 0;
	      do{

		  taskDelay(50);
		  printf("Waiting on nonzero aft hndshk: fore= %d aft= %d\n",
			 fore_vmehndshk->dc_remove_hndshk,
			 aft_vmehndshk->dc_remove_hndshk);

	      }while(aft_vmehndshk->dc_remove_hndshk == 0);
	  }while(aft_vmehndshk->dc_remove_hndshk != 2);
      }

    else if((fore_vmehndshk->dc_remove_hndshk != 2) && 
	   (aft_vmehndshk->dc_remove_hndshk == 2))
      {
	  do{
	      fore_vmehndshk->dc_remove_hndshk = 0;
	      do{

		  taskDelay(50);
		  printf("Waiting on nonzero fore hndshk: fore= %d aft= %d\n",
			 fore_vmehndshk->dc_remove_hndshk,
			 aft_vmehndshk->dc_remove_hndshk);

	      }while(fore_vmehndshk->dc_remove_hndshk == 0);
	  }while(fore_vmehndshk->dc_remove_hndshk != 2);
      }


    printf("Leaving DC remove Handshake\n");

    /* Clear the radar processors are ready handshake words */

    fore_vmehndshk->start_hndshk = 0;
    aft_vmehndshk->start_hndshk = 0;

    /* Clear the "Hi Radar" in the radar processors
                   to get them  to stop */

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

    /* Get the radar processors running again */

    aft_vmehndshk->dc_remove_hndshk = 0;
    fore_vmehndshk->dc_remove_hndshk = 0;

}
