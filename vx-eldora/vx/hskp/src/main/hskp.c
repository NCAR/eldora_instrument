/*
 *	$Id$
 *
 *	Module:	hskp	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
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
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
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
#include "hskpFunc.h"
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
#include "vme_hndshk.h"
#include "vmevmeAdr.h"
#include "vmevmeFunc.h"
#include "vmevmeGbl.h"
#include "iruDef.h"
#include "iruFunc.h"
#include "iruGbl.h"

void hskp(short rotation_flag)
{

/* Define some general purpose variables */
long kill, i;
float rpm;
unsigned long T;
double frequency, temp;
unsigned char ecbaddr, unitnum, filternum, test;
int onedone, timeout;

/* initialize general purpose variables */
fake_angles = rotation_flag;
kill = 1;
long kill;
/* Initialize the global control variables */

stop_flag = 1;
reload_flag = 0;
in_vmevme_isr = 0;
in_gps_isr = 0;

/* Initialize all of the various houskeeping interfaces */

printf("Initializing the clock card\n");
init_clock((short)244); /* Sets up the pointers to go with the clock card */

/* init_mini(); */   /* Zeros all the counters and disables the gimbal motor
                    sets up the clock interrupt */

printf("Initializing the VME to VME interfaces\n");
init_vmevme();  /* Intializes the VME to VME interface handshake area */

printf("Initializing the motor controller\n");
init_motor();   /* Sets gains, sample interval etc. on motor controller card */

/* init_ieee(); */    /* Initalizes the power meters to begin sending data */

printf("Initializing the ARINC 429 interface\n");
init_iru((short)0);     /* Initializes the ARINC 429 card to interrupt on
			   300 words or the latitude label */

printf("Initializing the GPS interface\n");
init_gps((short)0);/* Sets up the the GPS mailbox interrupt, proper pointers */

/* Enable 68040 Interrupts */
sysIntEnable(VME_VME_IRQ);
sysIntEnable(IEEE_IRQ);
sysIntEnable(ARINC_IRQ);
sysIntEnable(GPS_IRQ);


/********************************************************/
/**********  The code below is run once on a   **********/
/**********     STOP or a RELOAD Command       **********/
/********************************************************/

do{

    reload_flag = 0;

    printf("Stopping the VME to VME interfaces\n");
    stop_vmevme();
    printf("Stopping the motor controller\n");
    stop_motor();
    printf("Stopping the GPS interface\n");
    command_gps((char)3);

/*    stop_ieee(); */


    /* Wait here to be (re)started by the control processor */

    printf("Will wait now for the control processor to start me\n");
    do{
    taskDelay(60);
     }while(stop_flag);
    printf("Was started by the control processor\n");

    do{}while(stop_flag);
    araddes = GetRadar(inHeader,(int)2);
    vol = GetVolume(inHeader);
    wave = GetWaveform(inHeader);
    frad = GetFieldRadar(inHeader);
    vol = GetVolume(inHeader);
    insitdes = GetInsitu(inHeader);
    raddes = GetRadar(inHeader,(int)1);
  /*  frad = GetFieldRadar(inHeader); */
    /* Calculate the number of milliseconds in a dwell time */

/*    navdes = GetNav(inHeader);

    /* Set motor to new RPM's and start spinning */

    printf("Starting the motor\n");
    rpm = fraddes->req_rotat_vel * DEGPERSEC_TO_RPM;
    rpm = raddes->req_rotat_vel;
    go_motor();

    /* Program the Intermediate Frequency Signal processors with the
       proper filters */

    /* Note that this programs the aft identical to the fore this code
       will have to change, if that no longer is desired */

    for(i=0; i<fraddes->num_freq_trans; i++)
      {
	  ecbaddr = ECBIFFOR;
	  unitnum = i+1;
	  filternum = frad->filter_num[i];
	  timeout = 0;
	  do
	    {
		timeout++;
	    }while((test = ecbSetIF(ecbaddr,unitnum,filternum) != 0) &&
		   timeout < 30000);
	  if(timeout >= 30000)
    sysIntEnable(VME_VME_IRQ);

	  ecbaddr = ECBIFAFT;
	  timeout = 0;

	    {
		timeout++;
	    }while((test = ecbSetIF(ecbaddr,unitnum,filternum) != 0) &&
		   timeout< 30000);
	  if(timeout >= 30000)
	    printf("Failed to set aft IF filter IF #%d",unitnum);
      }

    /* Program the receiver/exciter chassis with the proper frequencies */
    /* Do the fore radar first */

    ecbaddr = ECBRFFOR;
	if(!in_vmevme_isr)
	  {
	      printf("V");
	      in_vmevme_isr = 0;
	  }
		case 0:
		frequency = fraddes->freq1 * 1e9;
		break;

		case 1:
		frequency = fraddes->freq2 * 1e9;
		break;

		case 2:
		frequency = fraddes->freq3 * 1e9;
		break;

		case 3:
		frequency = fraddes->freq4 * 1e9;
		break;

		case 4:
		frequency = fraddes->freq5 * 1e9;
		break;
	    }
	  B = (unsigned char)(361 - (int)(frequency / 30.0e6));
	  temp = 15445.3333333333333 - (frequency / 703125.0);
	  temp = (16777216.0 / (B+1)) * temp;
	  T = (unsigned long)(temp + 0.5);
	  unitnum = i+1;
	  nr = 1;
	  timeout = 0;
	  do
	    {
		timeout++;
	    }while((test = ecbSetDDS(ecbaddr,unitnum,B,T,nr) != 0)
		   && timeout < 30000);
	  if(timeout >= 30000)
	    printf("Failed to set DDS frequency fore RX, freq num: %d",
		   unitnum);

      }

    /* Now do the aft radar */

    ecbaddr = ECBRFAFT;
    for(i=0; i<araddes->num_freq_trans; i++)
      {
	  switch(i)
	    {
		case 0:
		frequency = araddes->freq1 * 1e9;
		break;

		case 1:
		frequency = araddes->freq2 * 1e9;
		break;

		case 2:
		frequency = araddes->freq3 * 1e9;
		break;

		case 3:
		frequency = araddes->freq4 * 1e9;
		break;

		case 4:
		frequency = araddes->freq5 * 1e9;
		break;
	    }
	  B = (unsigned char)(361 - (int)(frequency / 30.0e6));
	  temp = 15445.3333333333333 - (frequency / 703125.0);
	  temp = (16777216.0 / (B+1)) * temp;
	  T = (unsigned long)(temp + 0.5);
	  unitnum = i+1;
	  nr = 1;
	  timeout = 0;
	  do
	    {
		timeout++;
	    }while((test = ecbSetDDS(ecbaddr,unitnum,B,T,nr) != 0)
		   && timeout < 30000);
	  if(timeout >= 30000)
	    printf("Failed to set DDS frequency aft RX freq num: %d",
		   unitnum);
      }

    /* Now start the automatic testpulse calibration scheme */
     start_testpulse(); 

    /* Start the interrupts from the ieee-488 board */
  /*  start_ieee(); */

    /* Tell the gps and iru interfaces to begin looking for data again */
    in_gps_isr = 0;
    printf("Starting the GPS interface\n");
    *tp41_mbox_0 = (char)0;
    command_gps((char)1);

    printf("Starting the IRU Interface");
    start_iru();
 
    /* Start the radar proccessors over the vme to vme interfaces */
    /* Note: the radar processors should always be the very last things
       to be started */

    printf("starting the VME to VME interfaces\n");
    in_vmevme_isr = 0;
    start_vmevme();

/**************************************************************/
/**********    The code below is run at all times    **********/
/**********  until a STOP or a RELOAD Command occurs **********/
/**************************************************************/

    do{
       onedone = 0;

       i = 0;
       do{
	   i ++;
       }
       while(fore_vmehndshk->polled == 0 && !stop_flag && !reload_flag);
       vmevme_isr();

       if(old_iru_interrupts != iru_rpntr->num_interrupts)
	 {
              iru_isr();
	      onedone = 1;
	  }

	/* Check on the status of the miniRIMS */

	/* status_mini(); */

        if((*gps_hndshk != (short)0) && (onedone == 0))
	  {
	      gps_isr();
	      onedone = 1;
	  }

	if((tp_dwell_count >= testpulse_max_count) && (onedone == 0))
	  {
	      update_testpulse();
	      onedone = 1;
	  }

       }while(!stop_flag && !reload_flag);

   }while(kill);
}
