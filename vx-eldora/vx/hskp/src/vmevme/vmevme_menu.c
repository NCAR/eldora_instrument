/*
 *	$Id$
 *
 *	Module: vmevme_menu.c		 
 *	Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.3  1992/09/17  16:43:12  craig
 * *** empty log message ***
 *
 * Revision 1.2  1992/09/08  22:56:42  craig
 * *** empty log message ***
 *
 * Revision 1.1  1992/09/03  20:26:34  craig
 * Initial revision
 *
 *
 * description:  Allows the user to individually test the VME to 
 *               VME interfaces and radar processor handshaking.
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

/* rpc, status, commands and logfile includes */
#include "HskpStatus.h"

void vmevme_menu()
{

int cmd, i, number, index, kill;
float position;
long offset;

/* First Initalize the cards */
init_vmevme();

/* Just sit in the menu until told to quit */

do{
    printf("\nVMEVME MENU CHOICES: Enter return for choices\n\n");
    getchar();
    printf("0) End VMEVME menu\n");
    printf("1) Initialize the VME to VME interfaces\n");
    printf("2) Start the VME to VME interfaces\n");
    printf("3) Stop the VME to VME interfaces\n");
    printf("4) Turn on the interrupt\n");
    printf("5) Turn off the interrupt\n");
    printf("6) Display Synchronization flags\n");
    printf("7) Display Test Pulse Information\n");
    printf("8) Display Special Record length Information\n");
    printf("9) Display Fore Radar Ray Handshake Information\n");
    printf("10) Display Aft Radar Ray Handshake Information\n");
    printf("11) Display Nav Handshake Information\n");
    printf("12) Display In-Situ Handshake Information\n");
    printf("13) Send Fore Radar Data Record Over the MCPL\n");
    printf("14) Send Aft Radar Data Record Over the MCPL\n");
    printf("15) Send Nav Data Record Over the MCPL\n");
    printf("16) Send In-situ Data Record Over the MCPL\n");
    printf("17) Display Control Status Information\n");
    printf("18) Start and pole for VME to VME interrupts\n");
    printf("19) Start and follow the radar proc\n");
    scanf("%d",&cmd);
    getchar();

    switch(cmd)
      {
	case 1:	/* Initialize the VME to VME interfaces */
	  init_vmevme();
	  break;

	case 2:	/* Start the VME to VME interfaces */
	  start_vmevme();
	  break;


	case 3:	/* Stop the VME to VME interfaces */
	  stop_vmevme();
	  break;

	case 4:	/* Turn on interrupts */
	  sysIntEnable(VME_VME_IRQ);
	  break;

	case 5:	/* Turn off interrupts */
	  sysIntDisable(VME_VME_IRQ);
	  break;

	case 6:	/* Display Syncronization Flags */
	  printf("FORE: %c%c%c%c%c%c%c%c\n",fore_vmehndshk->salute[0],
		 fore_vmehndshk->salute[1],fore_vmehndshk->salute[2],
		 fore_vmehndshk->salute[3],fore_vmehndshk->salute[4],
		 fore_vmehndshk->salute[5],fore_vmehndshk->salute[6],
		 fore_vmehndshk->salute[7]);
	  printf("AFT: %c%c%c%c%c%c%c%c\n",aft_vmehndshk->salute[0],
		 aft_vmehndshk->salute[1],aft_vmehndshk->salute[2],
		 aft_vmehndshk->salute[3],aft_vmehndshk->salute[4],
		 aft_vmehndshk->salute[5],aft_vmehndshk->salute[6],
		 aft_vmehndshk->salute[7]);
	  break;

	case 7:	/* Display Test Pulse Information */
	  printf("Test Pulse info                   FORE            AFT\n");
	  printf("Flag:         %4x       %4x\n",fore_vmehndshk->tpulse_flg,
		 aft_vmehndshk->tpulse_flg);
	  printf("Level (dbm):  %4.1f       %4.1f\n",
                  fore_vmehndshk->tpulse_level,aft_vmehndshk->tpulse_level);
	  printf("Distance:     %4d       %4d\n",
                  fore_vmehndshk->tpulse_dist,aft_vmehndshk->tpulse_dist);
	  printf("Width:        %4d       %4d\n",
                  fore_vmehndshk->tpulse_width,aft_vmehndshk->tpulse_width);
	  printf("Frequency (GHz):  %4.1f       %4.1f\n",
                  fore_vmehndshk->tpulse_freq,aft_vmehndshk->tpulse_freq);
	  printf("Attenuation (db): %4d         %4d\n",
                  fore_vmehndshk->tpulse_atten,aft_vmehndshk->tpulse_atten);
	  printf("Frequency Num:    %4d         %4d\n",
                 fore_vmehndshk->tpulse_freq_num,
		 aft_vmehndshk->tpulse_freq_num);
	  break;

	case 8:	/* Display Special record length Information */
	  printf("Record Length info                   FORE            AFT\n");
	  printf("Navigation      %5d       %5d\n",fore_vmehndshk->nav_length,
		 aft_vmehndshk->nav_length);
	  printf("Insitu          %5d       %5d\n",fore_vmehndshk->ads_length,
		 aft_vmehndshk->ads_length);
	  break;

	case 9:	/* Display Fore Radar Ray Handshake Information */
	  printf("Fore radar handshake area display:\n");
	  for(i=0; i<6; i++)
	    printf("%4x  ",fore_vmehndshk->radar_hndshk[i]);
	  printf("\n");
	  for(i=6; i<12; i++)
	    printf("%4x  ",fore_vmehndshk->radar_hndshk[i]);
	  printf("\n");
	  for(i=12; i<18; i++)
	    printf("%4x  ",fore_vmehndshk->radar_hndshk[i]);
	  printf("\n");
	  for(i=18; i<24; i++)
	    printf("%4x  ",fore_vmehndshk->radar_hndshk[i]);
	  printf("\n");
	  for(i=24; i<NUM_RADAR_HNDSHK; i++)
	    printf("%4x  ",fore_vmehndshk->radar_hndshk[i]);
	  printf("\n");
	  break;

	case 10: /* Display Aft Radar Ray Handshake Information */
	  printf("Aft radar handshake area display:\n");
	  for(i=0; i<6; i++)
	    printf("%4x  ",aft_vmehndshk->radar_hndshk[i]);
	  printf("\n");
	  for(i=6; i<12; i++)
	    printf("%4x  ",aft_vmehndshk->radar_hndshk[i]);
	  printf("\n");
	  for(i=12; i<18; i++)
	    printf("%4x  ",aft_vmehndshk->radar_hndshk[i]);
	  printf("\n");
	  for(i=18; i<24; i++)
	    printf("%4x  ",aft_vmehndshk->radar_hndshk[i]);
	  printf("\n");
	  for(i=24; i<NUM_RADAR_HNDSHK; i++)
	    printf("%4x  ",aft_vmehndshk->radar_hndshk[i]);
	  printf("\n");
	  break;

	case 11: /* Display Nav Handshake Information */
	  printf("Navigational handshake area display:\n");
	  printf("FORE:  %4x   %4x\n",fore_vmehndshk->nav_hndshk[0],
		 fore_vmehndshk->nav_hndshk[1]);
	  printf("AFT:  %4x   %4x\n",aft_vmehndshk->nav_hndshk[0],
		 aft_vmehndshk->nav_hndshk[1]);
	  break;

	case 12: /* Display Insitu Handshake Information */
	  printf("Insitu handshake area display:\n");
	  printf("FORE:  %4x   %4x\n",fore_vmehndshk->ads_hndshk[0],
		 fore_vmehndshk->ads_hndshk[1]);
	  printf("AFT:  %4x   %4x\n",aft_vmehndshk->ads_hndshk[0],
		 aft_vmehndshk->ads_hndshk[1]);
	  break;

	case 13: /* Send a Fore radar data record over the MCPL */
	  printf("Enter the index to use (0 to 26): ");
	  scanf("%d",&index);
	  getchar();
	  if(index < 0) index = 0;
	  if(index > 26) index = 26;
	  printf("\n Fore radar record #%d sent over MCPL\n",index);
	  fore_vmehndshk->radar_hndshk[index] = (short)1;
	  break;

	case 14: /* Send a Aft radar data record over the MCPL */
	  printf("Enter the index to use (0 to 26): ");
	  scanf("%d",&index);
	  getchar();
	  if(index < 0) index = 0;
	  if(index > 26) index = 26;
	  printf("\n Aft radar record #%d sent over MCPL\n",index);
	  aft_vmehndshk->radar_hndshk[index] = (short)1;
	  break;

	case 15: /* Send a Nav data record over the MCPL */
	  printf("Enter the index to use (0 or 1): ");
	  scanf("%d",&index);
	  getchar();
	  if(index < 0) index = 0;
	  if(index > 1) index = 1;
	  printf("\n Nav data record #%d sent over MCPL\n",index);
	  fore_vmehndshk->nav_hndshk[index] = (short)1;
	  break;

	case 16: /* Send an Insitu radar data record over the MCPL */
	  printf("Enter the index to use (0 to 1): ");
	  scanf("%d",&index);
	  getchar();
	  if(index < 0) index = 0;
	  if(index > 1) index = 1;
	  printf("\nInsitu data record #%d sent over MCPL\n",index);
	  fore_vmehndshk->ads_hndshk[index] = (short)1;
	  break;

	case 17:/* Display Control Status Information */
	  printf("Fore Radar processor status %x\n",currStatus->fore);
	  printf("Aft Radar processor status %x\n",  currStatus->aft);
	  printf("Interrupt Detected Flag %d   \n",in_vmevme_isr);
	  break;

	case 18: /* pole VME to VME Interrupts */
	  kill = 0;
	  fore_vmehndshk->polled = 0;
	  aft_vmehndshk->polled = 0;
	  start_vmevme();
	  do{
	       if(fore_vmehndshk->polled != 0)
		 vmevme_isr();
	       }while(kill == 0);
	  break;

	case 19: /* Start and follow the radar processor */
	  index = 0;
	  offset = FIRST_RADAR_OFFSET;
	  kill = 0;
	  start_vmevme();
          do{
	      if((unsigned short)fore_vmehndshk->
                   radar_hndshk[index] != 0xAFFF)
	    {
	        printf("No AFFF seen index = %d value = %d\n",
                  index,fore_vmehndshk->
                   radar_hndshk[index]);
		kill = 1;
	    }
	      do{
		  i++;;
	      }while((unsigned short)fore_vmehndshk->
		     radar_hndshk[index] != 0);

	  fore_ray_pntr = (struct DATARAY *)(offset +
				    STANDARD_BASE + FORE_STAND_START); 
	  position += 1.8;
	  if(position > 360.0) position = 0.0;
	  fore_ray_pntr->this_plat.rotation_angle = position;
	  fore_ray_pntr->this_rayi.true_scan_rate = 20.0;

	  fore_vmehndshk->
		     radar_hndshk[index] = 1;


	  offset += RADAR_SIZE_INCR;
	  index++;
	  if(index >= NUM_RADAR_HNDSHK)
	    {
		offset = FIRST_RADAR_OFFSET;
		index = 0;
	    }
/*	  printf("%2d",index); */

	  }while(kill == 0);
	  break;

    default:
      break;
  }
}while(cmd != 0);

return;
}
