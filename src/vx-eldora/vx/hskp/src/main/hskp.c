/*
 * $Id$
 *
 * Module: hskp  
 * Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 * Date:   $Date$
 *
 * revision history
 * ----------------
 * $Log: hskp.c,v $
 * Revision 1.19  2005/08/04 22:27:37  granger
 * commit vx/hskp as copied from /net/eldora/eldora, except a few obsolete
 * (afaik) directories were removed, like hskp/src/clock/newclk and
 * hskp/src/arinc_NCAR
 *
 * Revision 1.18  2003/10/02  15:14:30  kapoor
 * Changed back to normal TOD function calls, will also work Bancomm clock.
 *
 * Revision 1.14  2000/05/15  18:30:25  eric
 * Revised power meter stop routine and added firstime flag so
 * that stop_ieee is not called upon powerup.
 *
 * Revision 1.13  1999/09/27  15:43:46  eric
 * added taskDelay in run-time loop to improve chances for slow
 * cal to run.
 *
 * Revision 1.12  1996/12/09  22:58:34  eric
 * Added support for Minirims interface.
 *
 * Revision 1.11  1996/12/09  22:43:13  craig
 * new iru interface
 *
 * Revision 1.10  1996/10/29  22:53:21  craig
 * *** empty log message ***
 *
 * Revision 1.9  1996/07/23  20:18:44  root
 * Saving Craig's latest work.
 *
 * Revision 1.8  1996/02/09  18:29:49  craig
 * *** empty log message ***
 *
 * Revision 1.7  1994/05/20  20:37:20  craig
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

static char rcsid[] = "$Date$ $RCSfile: hskp.c,v $ $Revision$";


#define scope extern
#include "hskpAll.h"

extern void stop11(void);
extern void dpclr(void);
extern void go11(void);
extern short ldsrec(char*);
static int First;
void hskp()
{


/* Define some general purpose variables */

#define DEGS_TO_RADS 0.0174533
long kill, i;
unsigned char B, nr;
unsigned long T, aint;
double frequency, temp;
unsigned char ecbaddr, unitnum, filternum, test;
int timeout;
char ecbname[20];
float anglesin;
float dumb_rads, dumb_stepr, dumb_stepp;
int dumb_start, dumb_index;

printf("\nStarting the Housekeeper Code Now...\n\n");
getchar();

/* initialize general purpose variables */
First = 1;
kill = 1;
dumb_start = 0;
dumb_index = 25;
autocal = 1;

//Tom 3/20/08
fake_intrs = 0;

/* Initialize the global control variables */

stop_flag = 1;
reload_flag = 0;
in_vmevme_isr = 0;
in_gps_isr = 0;

use_IRIGB = 1;
modulate_IRIGB = 1;

/* Initialize all of the various houskeeping interfaces */

//*******************************
//Edited by Tom 1/30/08

//printf("Initializing the ELDORA Control Bus (ecb)\n");

//strcpy(ecbname,"mstrvme8.sre");
stop11();
//dpclr();
//ldsrec(ecbname);
//ecbIntInit(1000000);
go11();
//*******************************

/* Enable 68040 Interrupts */

sysIntEnable(VME_VME_IRQ);
sysIntEnable(IEEE_IRQ);

//*******************************
//Edited by Tom 1/30/08

sysIntEnable(GPS_IRQ);
sysIntEnable(ECB_CMPLT_IRQ);
sysIntEnable(ECB_ERROR_IRQ);
sysIntEnable(ECB_SPARE_IRQ);
//*******************************

//Tom 3/26/08
printf("Initializing the UDP Interface\n");
init_UDP();

printf("Initializing the clock card\n");
init_clock((short)244); /* Sets up the pointers to go with the clock card */

/* init_mini(); */    /* Initializes and connects mailbox interrupt */ 


printf("Initializing the VME to VME interfaces\n");
init_vmevme();  // Intializes the VME to VME interface handshake area //

printf("Initializing the motor controller\n");
init_motor();   /* Sets gains, sample interval etc. on motor controller card */

init_ieee(); /* Initalizes the power meters to begin sending data */

printf("Initializing the ARINC 429 interface\n");
init_iru();     /* Initializes the ARINC 429 card to sort on labels */

printf("Initializing the GPS interface\n");
init_gps((short)0); // Sets up the the GPS mailbox interrupt, proper pointers //


/* Initialize parameters to deal with the waveguide switch */
wg_sw_flag = 0;
wg_sw_pntr = (unsigned char *)(SHORT_BASE + WG_SW_BASE);

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
        
    /*    command_mini((short)3);  */ /* disable Mailbox interrupt */

    /* Added the following two lines to try to improve power meter reliability */
    if(!First)
      {
 autocal = 0;
 taskDelay(20);
 stop_ieee(); 
      }
    First = 0;
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

    
    // Set mcpl error variable to no error condition //
    mcpl_error[0] = 1;
    mcpl_error[1] = 1;
    mcpl_error[2] = 0;

    // Tom 3/19/08 Initialize Info Blocks (use to be done in RP7)
    printf("Initializing HSKP Info Blocks\n");
    init_info_blocks();
       
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

    
    //***************************************
    /*
    Edited by Tom 1/30/08

    // Program the receiver/exciter chassis with the proper frequencies //
    // Do the fore radar first //

    ecbaddr = ECBRFFOR;
    for(i=0; i<fraddes->num_freq_trans; i++)
      {
   switch(i)
     {
  case 0:
  aint = (double)fraddes->freq1 * (double)1000000;
  frequency = (double)aint * (double)1000.0;
  break;

  case 1:
  aint = (double)fraddes->freq2 * (double)1000000;
  frequency = (double)aint * (double)1000.0;
  break;

  case 2:
  aint = (double)fraddes->freq3  * (double)1000000;
  frequency = (double)aint * (double)1000.0;
  break;

  case 3:
  aint = (double)fraddes->freq4 * (double)1000000;
  frequency = (double)aint * (double)1000.0;
  break;

  case 4:
  aint = (double)fraddes->freq5 * (double)1000000;
  frequency = (double)aint * (double)1000.0;
  break;
     }
   fore_freqs[i] = frequency;
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
   else
     printf("Fore radar DDS #%1d set to %f Hertz\n",unitnum,frequency);
   taskDelay(60);

      }

    // Now do the aft radar //

    ecbaddr = ECBRFAFT;
    for(i=0; i<araddes->num_freq_trans; i++)
      {
   switch(i)
     {
  case 0:
  aint = (double)araddes->freq1 * (double)1000000;
  frequency = (double)aint * (double)1000.0;
  break;

  case 1:
  aint = (double)araddes->freq2 * (double)1000000;
  frequency = (double)aint * (double)1000.0;
  break;

  case 2:
  aint = (double)araddes->freq3 * (double)1000000;
  frequency = (double)aint * (double)1000.0;
  break;

  case 3:
  aint = (double)araddes->freq4 * (double)1000000;
  frequency = (double)aint * (double)1000.0;
  break;

  case 4:
  aint = (double)araddes->freq5 * (double)1000000;
  frequency = (double)aint * (double)1000.0;
  break;
     }
   aft_freqs[i] = frequency;
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
   else
     printf("Aft radar DDS #%1d set to %f Hertz\n",unitnum,frequency);
   taskDelay(60);
      }
    */
    //****************************************
    
    // Now start the automatic testpulse calibration scheme //
   
    autocal = 1;
    start_testpulse();

    
    // Initialize the mcpl handshake //
    fore_vmehndshk -> mcpl_hndshk = 1;
    aft_vmehndshk -> mcpl_hndshk = 0;
    mcpl_xfer_processor = 0;


    // Tell the gps and iru interfaces to begin looking for data again //
    in_gps_isr = 0;
    printf("Starting the GPS interface\n");
    *tp41_mbox_0 = (char)0;
    command_gps((char)1);
    //    command_mini((short)1); */ /* enable Mailbox interrupt //
       

    printf("Starting the IRU Interface\n");
    start_iru();
 
    /* Start the interrupts from the ieee-488 board */
   // Tom 3/6/08 GPIB not connected so comment out for now 
   //start_ieee(); 

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

  // Create fake interrupts every 1 second - Added Tom 3/7/08
  if(fake_intrs)
  {
    //printf("Interrupting Now.\n");
    semGive(vmeSem);
    taskDelay(60);
  }
        
        //***************************************
        /*
        Edited by Tom 1/30/08

 // Report MCPL errors (or radar processor handshake errors)
 //   that were discovered in the vmevme ISR but can not be
 //          reported until here //

 if(mcpl_error[0] != 1)
   // Fore radar handshake not set, or occured more than once between
    interrupts //
   {
       printf("F%1d",mcpl_error[0]);
       mcpl_error[0] = 1;
   }
 if(mcpl_error[1] != 1 && mcpl_error[1] != 0)
   // aft radar handshake occured more than once between interrupts//
   {
       printf("A%1d",mcpl_error[1]);
       mcpl_error[1] = 1;
   }

 if(mcpl_error[2] != 0)
   // Radar processors have no data available //
   {
       printf("N");
       mcpl_error[2] = 0;
   }

       // If new GPS data exists handle the data //

        if(*gps_hndshk != (short)0)
       gps_isr();
       */
       //****************************************
       

 //Tom 3/28/08 Removed for testing purposes - Fore Power not hooked up.
 //if(tp_dwell_count >= testpulse_max_count && autocal)
 //      update_testpulse();
 
 taskDelay(1);  /* added for test - Eric 7/16/99 */ 
       }while(!stop_flag && !reload_flag);

   }while(kill);
}

