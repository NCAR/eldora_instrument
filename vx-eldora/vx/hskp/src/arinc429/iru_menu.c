/*
 *	$Id$
 *
 *	Module: iru_menu.c		 
 *	Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/09/01  15:56:57  craig
 * Initial revision
 *
 * description:  Allows the user to individually test the IRU interface.
 *        
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define scope

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
#include "iruDef.h"
#include "iruGbl.h"
#include "iruFunc.h"
#include "minDef.h"
#include "minFunc.h"
#include "tp41vAdr.h"
#include "vmevmeDef.h"
#include "vmevmeAdr.h"
#include "vmevmeFunc.h"
#include "vmevmeGbl.h"

/* rpc, status, commands and logfile includes */
#include "HskpStatus.h"

void iru_menu()
{
long *lptr, *ptr1, *ptr2, i, number;
int cmd;

init_iru((short)0);

/* Just sit in the menu until told to quit */

do{
    printf("\nIRU MENU CHOICES:\n\n");
    printf("0) End IRU menu\n");
    printf("1) Start IRU interface\n");
    printf("2) Initialize IRU Interface With Interrupts\n");
    printf("3) Initialize IRU Interface Without Interrupts\n");
    printf("4) Display Lastest IRU Information\n");
    printf("5) Display Buffer #1\n");
    printf("6) Display Buffer #2\n");
    printf("7) Display Receiver Control Information\n");
    printf("8) Display Status Information\n");
    printf("9) Count IRU interrupts\n");
    scanf("%d",&cmd);
    getchar();
    printf("command entered: %d\n",cmd);

    switch(cmd)
      {
	case 1:	/* Start IRU interface */
	  start_iru();
	  start_vmevme();
	  break;

	case 2:	/* Initialize IRU interface with interrupts */
	  init_iru((short)0);
	  break;

	case 3:	/* Initialize IRU interface Turn on interrupts */
	  init_iru((short)1);
	  break;

	case 4:	/* Display the latest IRU info */

	  printf("seconds =         %d\n",last_iru_data.seconds);
	  printf("msec_longitude =  %d\n",last_iru_data.msec_longitude);
	  printf("status =          %d\n",last_iru_data.status);
	  printf("latitude =        %f\n",last_iru_data.latitude);
	  printf("longitude =       %f\n",last_iru_data.longitude);
	  printf("wind_speed =      %f\n",last_iru_data.wind_speed);
	  printf("wind_direction =  %f\n",last_iru_data.wind_direction);
	  printf("ns_velocity =     %f\n",last_iru_data.ns_velocity);
	  printf("ew_velocity =     %f\n",last_iru_data.ew_velocity);
	  printf("heading =         %f\n",last_iru_data.heading);
	  printf("drift =           %f\n",last_iru_data.drift);
	  printf("altitude =        %f\n",last_iru_data.altitude);
	  printf("inertial_vspeed = %f\n",last_iru_data.inertial_vspeed);
	  printf("pitch =           %f\n",last_iru_data.pitch);
	  printf("roll =            %f\n",last_iru_data.roll);
	  printf("integ_vert_acc =  %f\n",last_iru_data.integ_vert_acc);
	  printf("vert_acc =        %f\n",last_iru_data.vert_acc);
	  printf("track_rate =      %f\n",last_iru_data.track_rate);
	  printf("pitch_rate =      %f\n",last_iru_data.pitch_rate);
	  printf("Depress Enter to continue\n");
	  getchar();
	  break;

	case 5:	/* Display Buffer #1 */

	  lptr = &iru_rpntr->buf1[0];
	  for(i=0; i<300; i+=3)
	    {
		ptr1 = (long *)(lptr + 4);
		ptr2 = (long *)(lptr + 8);

		printf("%3d %8x %8x %8x\n",i,*lptr,*ptr1,*ptr2);
		lptr +=3;
	      }
	  printf("Depress Enter to continue\n");
	  getchar();
	  break;

	case 6:	/* Display Buffer #2 */

	  lptr = &iru_rpntr->buf2[0];
	  for(i=0; i<300; i+=3)
	    {
		ptr1 = (long *)(lptr + 4);
		ptr2 = (long *)(lptr + 8);

		printf("%3d %8x %8x %8x\n",i,*lptr,*ptr1,*ptr2);
		lptr +=3;
	      }
	  printf("Depress Enter to continue\n");
	  getchar();
	  break;

	case 7:	/* Display Control Area */
	  printf("max_words =      %d\n",iru_rpntr->max_words);
	  printf("words_buf1 =     %d\n",iru_rpntr->words_buf1);
	  printf("words_buf2 =     %d\n",iru_rpntr->words_buf2);
	  printf("int_flag =       %d\n",iru_rpntr->int_flag);
	  printf("term_label =     %x\n",iru_rpntr->term_label);
	  printf("num_interrupts = %d\n",iru_rpntr->num_interrupts);
	  break;

	case 8:	/* Display Control Status Information */
	  printf("IRU status word %x           \n",currStatus->iru);
	  printf("Local Interrupt counter %d   \n",old_iru_interrupts);
	  printf("Interrupt Detected Flag %d   \n",in_iru_isr);
	  break;

	case 9:	/* Count IRU Interrupts */
	  printf("Enter number to count\n");
	  scanf("%d",&number);
	  getchar();
	  for(i=0; i<number; i++)
	    {
		old_iru_interrupts = iru_rpntr->num_interrupts;
		do{}while(old_iru_interrupts == iru_rpntr->num_interrupts);
		iru_isr();
		printf("IRU interrupt #%d detected\n",i);
		in_iru_isr = 0;
	    }
	  break;

    default:
      break;
  }
}while(cmd != 0);

return;
}
