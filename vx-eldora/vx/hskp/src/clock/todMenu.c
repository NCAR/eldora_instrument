/*
 *	$Id$
 *
 *	Module:	todMenu.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/08/14  21:42:46  reif
 * Initial revision
 *
 *
 * description: The purpose of this module is to test the various functions
 *              of the TIME OF DAY board (TOD).  These include setting the
 *              Real Time Clock (RTC), writing
 *              and reading the various registers on the RTC, setting up the 
 *              TOD for interrupts using the Bus Interrupter Module (BIM), the 
 *              RTC and the Power Monitor (PM) chip.  The software also
 *              allows the TOD to be set to IRIGB and reading time from the
 *              RTC.
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
#include "gpsFunc.h"
#include "minDef.h"
#include "minFunc.h"
#include "tp41vAdr.h"
#include "vmevmeDef.h"
#include "vmevmeAdr.h"
#include "vmevmeFunc.h"
#include "vmevmeGbl.h"

/**************************************************************/

void tod_menu()
{
char hour, minute, second, aday, month, year, test;
short  millisec, julday;
unsigned char choice;

/* Initialize all of the global pointers */
init_clock((short)244);

do {
    puts("TOD UTILITY PROGRAM");
    puts("PRESS THE KEY OF YOUR CHOICE");
    puts("W)rite data to TOD board");
    puts("R)ead data from TOD board");
    puts("S)ynch TOD to IRIGB");
    puts("M)anually set clock");
    puts("B)egin the clock ticking");
    puts("G)et time");
    puts("Q)uit");
    choice = getchar();
    getchar();
    switch(choice) {
      case 'w':
	write_data();
	break;
      case 'r':
	read_data();
	break;
      case 's':
	if(test = sync_irig())
	  printf("IRIG-B syncing was unsuccessful\n");
	break;
      case 'm':
	printf("\nEnter the hour in 24 hour format: ");
	scanf("%d",&hour);
	printf("\nEnter the minutes: ");
	scanf("%d",&minute);
	printf("\nEnter the seconds: ");
	scanf("%d",&second);
	printf("\nEnter the day of month: ");
	scanf("%d",&aday);
	printf("\nEnter the month: ");
	scanf("%d",&month);
	printf("\nEnter the year: ");
	scanf("%d",&year);
	settime(hour,minute,second,month,aday,year);
	printf("\nDepress any key to start the clock\n");
	getchar();
	start_clock();
	break;
      case 'g':
	get_time(&hour,&minute,&second,&millisec,&julday,&month,&aday,&year);
	printf("\nTHE TIME IS %02d:%02d:%02d:%02d\n",hour,minute,
	       second,millisec);
	printf("THE DATE IS %02d/%02d/%02d\n",month,aday,year);
	printf("JULIAN DAY IS %d\n",julday);
	break;
      case 'q':
	break;
      default:
	puts("Not a choice. Press any key to resume");
	getchar();
	break;
    }
}while(choice != 'q');
}
