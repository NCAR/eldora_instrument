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
 * description:  Allows the user to individually test the GPS interface.
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

void gps_menu()
{

int buf_num, cmd;
struct gps_data *pntr;

init_gps((short)0);

/* Just sit in the menu until told to quit */

do{
    printf("GPS MENU CHOICES:\n\n");
    printf("0) End GPS menu\n");
    printf("1) Start GPS interface\n");
    printf("2) Stop GPS interface\n");
    printf("3) Turn on interrupts\n");
    printf("4) Initialize the GPS receiver\n");
    printf("5) Display Time Information\n");
    printf("6) Display Satellite Information\n");
    printf("7) Display Software Information\n");
    printf("8) Display Health Information\n");
    printf("9) Display Position Information\n");
    printf("10) Display Status Information\n");
    printf("11) Display Header Information\n");
    printf("12) Display Handshake Information\n");
    printf("13) Display Control Status Information\n");
    scanf("%d",&cmd);
    if((cmd > 4) && (cmd < 12))
      {
	  printf("From which buffer? (0,1)\n");
	  scanf("%d",&buf_num);
	  if(buf_num = 0)
	    pntr = (struct gps_data *)(STANDARD_BASE + GPS_BASE + 0x10020);
	  else
	    pntr = (struct gps_data *)(STANDARD_BASE + GPS_BASE + 0x10100);
      }

    switch(cmd)
      {
	case 1:	/* Start GPS interface */
	  command_gps((short)1);
	  break;

	case 2:	/* Stop GPS interface */
	  command_gps((short)3);
	  break;

	case 3:	/* Turn on interrupts */
	  init_gps((char)1);
	  break;

    case 4:	/* Initialize the GPS receiver */
	  command_gps((short)2);
	  break;

    case 5:	/* Display Time Information */
	  printf("Milli Seconds last fix = %d\n",pntr->msec_fix);
	  printf("Week Number =            %d\n",pntr->week_number);
	  printf("Seconds of Week =        %ld\n",pntr->secs_of_week);
	  printf("UTC offset =             %ld\n",pntr->utc_offset);
	  break;

    case 6:	/* Display Satellite Information */
	  printf("Mode as Defined By Trimbal = %lx\n",pntr->gps_mode);
	  printf("Satellite number 1 =         %d\n",pntr->satellite[0]);
	  printf("Satellite number 2 =         %d\n",pntr->satellite[1]);
	  printf("Satellite number 3 =         %d\n",pntr->satellite[2]);
	  printf("Satellite number 4 =         %d\n",pntr->satellite[3]);
	  printf("PDOP =                       %f\n",pntr->pdop);
	  printf("HDOP =                       %f\n",pntr->hdop);
	  printf("VDOP =                       %f\n",pntr->vdop);
	  printf("TDOP =                       %f\n",pntr->tdop);
	  break;

    case 7:	/* Display Software Information */
	  printf("Software version = %d\n",pntr->software_version);
	  printf("Month =            %d\n",pntr->month);
	  printf("Day =              %d\n",pntr->day);
	  printf("Year =             %d\n",pntr->year);
	  break;

    case 8:	/* Display Health Information */
	  printf("Health as defined by Trimbal =     %x\n",pntr->health);
	  printf("Machine ID as defined by Trimbal = %x\n",pntr->machine_id);
	  break;

    case 9:	/* Display Position Information */
	  printf("Latitude (deg) =  %f\n",pntr->latitude);
	  printf("Longitude (deg) = %f\n",pntr->longitude);
	  printf("altitude (m) =    %f\n",pntr->altitude);
	  break;

    case 10:	/* Display Status Information */
	  printf("Clock Bias =  %f\n",pntr->clock_bias);
	  printf("Time of Fix = %f\n",pntr->time_of_fix);
	  break;

    case 11:	/* Display Header Information */
	  printf("GPS data ID = %c%c%c%c\n",pntr->gps_data_id[0],
               pntr->gps_data_id[1],pntr->gps_data_id[2],pntr->gps_data_id[3]);
	  printf("Size of data area (bytes) = %d\n",pntr->gps_data_len);
	  break;

    case 12:	/* Display Handshake Information */
	  printf("CPU-105 GPS status:        %x\n",*gps_status);
	  printf("CPU-105 GPS Data Location: %lx\n",*gps_data_loc);
	  printf("CPU-105 GPS command:       %x\n",*gps_command);
	  printf("CPU-105 GPS mailbox flag:  %x\n",*gps_mbox);
	  printf("CPU-105 GPS handshake:     %x\n",*gps_hndshk);
	  break;

    case 13:	/* Display Control Status Information */
	  printf("Fore Radar processor status %x\n",currStatus->fore);
	  printf("Aft Radar processor status %x\n",  currStatus->aft);
	  printf("GPS status word %x\n",             currStatus->gps);
	  break;

    case 14:	/* Clear the handshake word */
	  printf("Clearing handshake word\n");
	  *gps_hndshk = (short)0;
	  break;

    default:
      break;
  }
}while(cmd != 0);

return;
}
