/*
 *	$Id$
 *
 *	Module:	DC_ADJUST	 
 *	Original Author: Eric Loew
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1993/06/09  15:08:49  eric
 * Added handshaking with housekeeper during dc_removal to help
 * improve synchronization.
 *
 * Revision 1.1  1992/11/09  16:07:51  eric
 * Initial revision
 *
 *
 * description: This module provides performs dc offset removal for the 
 *              selected ELDORA RP7 A/D Card and channel.
 *              
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define NO_RP7_SCOPE

/* define conventions used for prog_calram, prog_choiceram, etc..  */
#define    I       (char)1
#define    Q       (char)0
#define    HI      (char)0
#define    LO      (char)1

/* defines for choice ram to set either high or low channel */
#define    LOW     2895L
#define    HIGH    0L

/* miscellanious defines */
#define    THRESHOLD  648L
#define    N       1000

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

#include "RadarGbls.h"
#include "mcpl_gbl.h"
#include "ELDRP7.h"
#include "coll_dpramdefs.h"
#include "colquadsubs.h"
#include "dspdefs.h"
#include "dspaddr.h"
#include "rp7.h"

typedef  struct {float x,y;} complex;

complex       a[N];

int dc_adjust(f1_flag,f2_flag,f3_flag)
int  f1_flag, f2_flag, f3_flag;

{

int     i, sem_status;
float   bi,bq,mi,mq;
unsigned long  board;

void prog_calram(unsigned long,unsigned long,char,char,float,float);
void prog_choiceram(unsigned long,unsigned long,long);
void prog_admisc(unsigned long,unsigned long,short,char,char);

/* Wait for handshake from housekeeper */
vme2_pntr->dc_remove_hndshk = 1;
while(vme2_pntr -> dc_remove_hndshk != 0)
  {
      i++;
  }

timoffb();
dspstop(1,4,0,0);
if(f1_flag)
  {
      dspstop(1,1,3,-1);
      dspstop(1,1,2,-1);
      dspstop(1,1,1,-1);
  }

if(f2_flag)
  {
      dspstop(1,2,3,-1);
      dspstop(1,2,2,-1);
      dspstop(1,2,1,-1);
  }
			
if(f3_flag)
  {
      dspstop(1,3,3,-1);
      dspstop(1,3,2,-1);
      dspstop(1,3,1,-1);
  }

/* Clear DP Out of Sync Interrupt that may occur as a result of the 
	 stoppage  */

sem_status = semTake(bim_int2_sem, NO_WAIT); /* don't wait for ISR to pass sem */

if (sem_status == OK)
  {
      *bim_cr2 = 0xda;  /* re-enable interrupt INT2* */            
  }
/* Clear Collator Out of Sync Interrupt that may occur as a result of 
         the stoppage */

sem_status = semTake(bim_int3_sem, NO_WAIT); /* don't wait ISR to pass sem */
if (sem_status == OK)
  {
      *bim_cr3 = 0xd9;  /* re-enable interrupt INT3* */            
  }

/* 
    Try taking mid and end of beam semaphores in case they
    were given prior to Stop command and not taken
*/

sem_status = semTake(exec_int0_sem,NO_WAIT);
sem_status = semTake(bim_int1_sem,NO_WAIT);
if(sem_status == OK)
  *bim_cr1 = 0xdb;  /* re-enable interrupt INT1* */



board = 4;  /* hardwired for RP7 ADC's */
   
/* set choice ram to select only desired channel */
		  
prog_choiceram(ad_freq,board,ad_chan?LOW:HIGH );

/* program CAL RAM for known gain and offset */

prog_calram(ad_freq,board,ad_chan,I,1.0,0.0);
prog_calram(ad_freq,board,ad_chan,Q,1.0,0.0);    

dspgo(1,4,0,0);
taskDelay(1);
if(f1_flag)
  {
      dspgo(1,1,3,3);
      dspgo(1,1,3,2);
      dspgo(1,1,3,1);
      dspgo(1,1,3,0);
      taskDelay(1);
      dspgo(1,1,2,-1);
      dspgo(1,1,1,-1);
  }
if(f2_flag)
  {
      dspgo(1,2,3,3);
      dspgo(1,2,3,2);
      dspgo(1,2,3,1);
      dspgo(1,2,3,0);
      taskDelay(1);
      dspgo(1,2,2,-1);
      dspgo(1,2,1,-1);
  }
if(f3_flag)
  {
      dspgo(1,3,3,3);
      dspgo(1,3,3,2);
      dspgo(1,3,3,1);
      dspgo(1,3,3,0);
      taskDelay(1);
      dspgo(1,3,2,-1);
      dspgo(1,3,1,-1);
  }
			
taskDelay(5); /* ensure all dsp's have completed their initializations */
/* Wait for handshake from housekeeper */

vme2_pntr->dc_remove_hndshk = 1;
while(vme2_pntr -> dc_remove_hndshk != 0)
  {
      i++;
  }


timonb();
/* Wait for Radar Processor Pipeline to fill */

for(i=0;i<3;i++)
  {
      /* Wait 0.3 seconds maximum for MID-BEAM INTERRUPT */

      sem_status = semTake(bim_int1_sem, 30); /* wait 30 ticks for ISR to pass sem */
      if (sem_status == OK)
	{
	    *bim_cr1 = 0xdb;  /* re-enable interrupt INT1* */

	}
      else
	{
	    printf("ERROR:  NO MID-BEAM INTERRUPT RECEIVED \n");
	    return(1);  /* return to main exec */
	    /* Update Alarm Status */
	    /* Call Alarm */
	    
	}

/* Wait 0.3 seconds for End of Beam Interrupt */

      sem_status = semTake(exec_int0_sem, 30); /* wait 30 ticks for ISR to pass sem */

      if (sem_status != OK)
	{
	    printf("ERROR:  NO END OF BEAM INTERRUPT RECEIVED \n");
	    return(1);  /* return to main exec */
	    /* Update Alarm Status */
	    /* Call Alarm */
	    
	}
      /* Test for DP Out of Sync Interrupt */

      sem_status = semTake(bim_int2_sem, NO_WAIT); /* don't wait for ISR to pass sem */

      if (sem_status == OK)
	{
	    *bim_cr2 = 0xda;  /* re-enable interrupt INT2* */            
	    printf("Doppler Processor Out of Sync Error \n");
/*	    return(1); */ /* return to main exec */
	}
      /* Test for Collator Out of Sync Interrupt */

      sem_status = semTake(bim_int3_sem, NO_WAIT); /* don't wait ISR to pass sem */
      if (sem_status == OK)
	{
	    *bim_cr3 = 0xd9;  /* re-enable interrupt INT3* */            
	    printf("Collator Out of Sync Error \n");
	/*    return(1); */ /* return to main exec */
	}

  }

/* Set up time series data collection */

a_pntr = (float *)a;
pts = N; 
fill_ts_array = 1;    /* fill time series array */

while(semTake(array_full_sem,NO_WAIT) != OK)    /* wait for data array to fill */
  {
      /* Wait 0.3 seconds maximum for MID-BEAM INTERRUPT */

      sem_status = semTake(bim_int1_sem, 30); /* wait 30 ticks for ISR to pass sem */
      if (sem_status == OK)
	{
	    *bim_cr1 = 0xdb;  /* re-enable interrupt INT1* */

	}
      else
	{
	    printf("ERROR:  NO MID-BEAM INTERRUPT RECEIVED -- AFTER TS FILL \n");
	    fill_ts_array = 0;
	    return(1);  /* return to main exec */
	    /* Update Alarm Status */
	    /* Call Alarm */
	    
	}


/* Wait 0.3 seconds for End of Beam Interrupt */

      sem_status = semTake(exec_int0_sem, 30); /* wait 30 ticks for ISR to pass sem */

      if (sem_status != OK)
	{
	    printf("ERROR:  NO END OF BEAM INTERRUPT RECEIVED -- AFTER TS FILL\n");
	    fill_ts_array = 0;
	    return(1);  /* return to main exec */
	    /* Update Alarm Status */
	    /* Call Alarm */
	    
	}
      /* Test for DP Out of Sync Interrupt */

      sem_status = semTake(bim_int2_sem, NO_WAIT); /* don't wait for ISR to pass sem */

      if (sem_status == OK)
	{
	    *bim_cr2 = 0xda;  /* re-enable interrupt INT2* */            
	    printf("Doppler Processor Out of Sync Error -- AFTER TS_FILL\n");
	    fill_ts_array = 0;
/*	    return(1); */ /* return to main exec */
	}
      /* Test for Collator Out of Sync Interrupt */

      sem_status = semTake(bim_int3_sem, NO_WAIT); /* don't wait ISR to pass sem */
      if (sem_status == OK)
	{
	    *bim_cr3 = 0xd9;  /* re-enable interrupt INT3* */            
	    printf("Collator Out of Sync Error -- AFTER TS_FILL\n");
	    fill_ts_array = 0;
/*	    return(1); */ /* return to main exec */
	}

  }
/* Wait for handshake from housekeeper */

vme2_pntr->dc_remove_hndshk = 1;
while(vme2_pntr -> dc_remove_hndshk != 0)
  {
      i++;
  }

timoffb();
dspstop(1,4,0,0);
if(f1_flag)
  {
      dspstop(1,1,3,-1);
      dspstop(1,1,2,-1);
      dspstop(1,1,1,-1);
  }

if(f2_flag)
  {
      dspstop(1,2,3,-1);
      dspstop(1,2,2,-1);
      dspstop(1,2,1,-1);
  }
			
if(f3_flag)
  {
      dspstop(1,3,3,-1);
      dspstop(1,3,2,-1);
      dspstop(1,3,1,-1);
  }

/* Clear DP Out of Sync Interrupt that may occur as a result of the 
	 stoppage  */

sem_status = semTake(bim_int2_sem, NO_WAIT); /* don't wait for ISR to pass sem */

if (sem_status == OK)
  {
      *bim_cr2 = 0xda;  /* re-enable interrupt INT2* */            
  }
/* Clear Collator Out of Sync Interrupt that may occur as a result of 
         the stoppage */

sem_status = semTake(bim_int3_sem, NO_WAIT); /* don't wait ISR to pass sem */
if (sem_status == OK)
  {
      *bim_cr3 = 0xd9;  /* re-enable interrupt INT3* */            
  }
/* 
    Try taking mid and end of beam semaphores in case they
    were given prior to Stop command and not taken
*/

sem_status = semTake(exec_int0_sem,NO_WAIT);
sem_status = semTake(bim_int1_sem,NO_WAIT);
if(sem_status == OK)
  *bim_cr1 = 0xdb;  /* re-enable interrupt INT1* */

a_pntr = (float *)a;
getb(a,N,&bi,&bq);

/* computed gain will depend on which channel is selected */
mi = mq = ad_chan?LOWm:HIGHm;

/* program CAL RAM with computed m' and b' */

prog_calram(ad_freq,board,ad_chan,I,mi,-bi*mi);
prog_calram(ad_freq,board,ad_chan,Q,mq,-bq*mq);

/* set up choice for standard threshold (for operations) */

prog_choiceram(ad_freq,board,THRESHOLD);
return(0);			
}

     






