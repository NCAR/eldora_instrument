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


#define scope extern
#include "hskpAll.h"

extern void stop11(void);
extern void dpclr(void);
extern void go11(void);
extern short ldsrec(char*);

void hskp()
{

/* Define some general purpose variables */

#define DEGS_TO_RADS 0.0174533
long kill, i;
unsigned char B, nr;
unsigned long T;
double frequency, temp;
unsigned char ecbaddr, unitnum, filternum, test;
int timeout;
char ecbname[20];
float anglesin;
static float dumb_rads, dumb_stepr, dumb_stepp;
static int dumb_start = 0, dumb_index = 25;


/* initialize general purpose variables */
kill = 1;

/* Initialize the global control variables */

stop_flag = 1;
reload_flag = 0;
in_vmevme_isr = 0;
in_gps_isr = 0;

/* Initialize all of the various houskeeping interfaces */

printf("Initializing the ELDORA Control Bus (ecb)\n");

strcpy(ecbname,"mstrvme8.sre");
stop11();
dpclr();
ldsrec(ecbname);
ecbIntInit(1000000);
go11();

/* Enable 68040 Interrupts */
sysIntEnable(VME_VME_IRQ);
sysIntEnable(IEEE_IRQ);
sysIntEnable(ARINC_IRQ);
sysIntEnable(GPS_IRQ);
sysIntEnable(ECB_CMPLT_IRQ);
sysIntEnable(ECB_ERROR_IRQ);
sysIntEnable(ECB_SPARE_IRQ);

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

    /* Set up all of the global header pointers */

    fraddes = GetRadar(inHeader,(int)1);
    araddes = GetRadar(inHeader,(int)2);
    vol = GetVolume(inHeader);
    wave = GetWaveform(inHeader);
    ffrad = GetFieldRadar(inHeader,1);
    afrad = GetFieldRadar(inHeader,2);
    cs = GetCellSpacing(inHeader,1);
    param = GetParameter(inHeader,(int)0);
    navdes = GetNavDesc(inHeader);
    insitdes = GetInsitu(inHeader);

    /* Calculate the number of milliseconds in a dwell time */

    dwelltime_msec = wave->repeat_seq * wave->repeat_seq_dwel;
    half_dwelltime_msec = dwelltime_msec / 2;

    /* Calculate the sine and cosine of each of the tilt angles */
    sin_ftilt = sin((double)(ffrad->E_plane_angle * DEGS_TO_RADS));
    sin_atilt = sin((double)(afrad->E_plane_angle * DEGS_TO_RADS));
    cos_ftilt = cos((double)(ffrad->E_plane_angle * DEGS_TO_RADS));
    cos_atilt = cos((double)(afrad->E_plane_angle * DEGS_TO_RADS));

    /* Set motor to new RPM's and start spinning */

    printf("Starting the motor\n");
    rpm = fraddes->req_rotat_vel * DEGPERSEC_TO_RPM;
    set_vel(rpm);
    go_motor();

    /* Program the receiver/exciter chassis with the proper frequencies */
    /* Do the fore radar first */
/*
    ecbaddr = ECBRFFOR;
    for(i=0; i<fraddes->num_freq_trans; i++)
      {
	  switch(i)
	    {
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
*/
    /* Now do the aft radar */
/*
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
*/
    /* Now start the automatic testpulse calibration scheme */
  /*   start_testpulse(); */ 

    /* Start the interrupts from the ieee-488 board */
  /*  start_ieee(); */

    /* Tell the gps and iru interfaces to begin looking for data again */
    in_gps_isr = 0;
    printf("Starting the GPS interface\n");
    *tp41_mbox_0 = (char)0;
    command_gps((char)1);

    printf("Starting the IRU Interface\n");
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

	/* Is there new IRU data to handle? */

       if(old_iru_interrupts != iru_rpntr->num_interrupts)
              iru_isr();

	/* Check on the status of the miniRIMS */

	/* status_mini(); */


       /* If new GPS data exists handle the data */

        if(*gps_hndshk != (short)0)
	      gps_isr();

/*	if(tp_dwell_count >= testpulse_max_count)
	      update_testpulse();
*/
       /* If fake_angles is true, we need to fake up some iru data */

/* Put in a fake parameters if global variable fake_angles is true */

       if(fake_angles)
	 {
	     if(dumb_start == 0)
	       {
		   dumb_start = 1;
		   dumb_stepr = 6.2831 / (60000.0 / (float)dwelltime_msec);
		   /* 2 pi radians per second */
		   dumb_stepp = 0.000757 / (float)dwelltime_msec;
		   /* Is about 120 m/s if lat step = long step */ 
		   dumb_rads = 0;
		   last_iru_data.longitude = -105.;
		   last_iru_data.latitude = 40.;
	       }

	     /* Dummy up the INS parameters */
	     dumb_rads += dumb_stepr;
	     if(dumb_rads > 6.28318) dumb_rads = 0;
	     anglesin = sin(dumb_rads);
	     last_iru_data.longitude += dumb_stepp;
	     last_iru_data.latitude += dumb_stepp;
	     last_iru_data.pitch = 2.0 * anglesin;
	     last_iru_data.roll = 15.0 * anglesin;
	     last_iru_data.heading = 45.0 + 0.5 * anglesin;
	     last_iru_data.altitude = 4.572 + 0.3048 * anglesin;
	     last_iru_data.seconds = sec;
	     last_iru_data.msec_longitude = msec;
	     dumb_index++;
	     if(dumb_index >= NUM_RADAR_HNDSHK) dumb_index=0;
	     fill_platform(msecs_ray[dumb_index]);

	 }

       }while(!stop_flag && !reload_flag);

   }while(kill);
}
