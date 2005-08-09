
/*
 *	$Id$
 *
 *	Module: mini_rt_rd.c		 
 *	Original Author: Eric Loew 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * Initial revision
 *
 *
 * description:
 *
 * This module reads minirims VME Card counters at a pre-programmed
 * rate (card firmware) and fills a double buffered data structure in
 * dual ported memory. When the structure is full a flag and/or a mailbox
 * interrupt is set to the host CPU in the housekeeping rack.
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define NO_MIN_SCOPE
#define NO_STAND_ALONE
#define TEMP
#define INTERRUPT
#define TIME
#define MAILBOX 0x303FF100
#define scope extern
#define int_vec2 175
#define MAX_CNT 512
#include "numinDef.h"
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include "minGbl.h"
#include "minDisp.h"
#include "string.h"
#include "semLib.h"
#define RES_SCALE  360.0/65536.0

/************************* MAIN PROGRAM *********************/
static SEM_ID mini_rd_sem;
static unsigned short *intsel;
static unsigned short *ltch_cnt;
static int count; 
void mini_rt_rd()
{

unsigned short *rd_tmp;
unsigned short *min_stat,stat;
unsigned short *res_stat;
unsigned short *x_accel;
unsigned short *y_accel;
unsigned short *z_accel;
unsigned short *x_vel_up;
unsigned short *y_vel_up;
unsigned short *z_vel_up;
unsigned short *xr_vel_up;
unsigned short *x_vel_dn;
unsigned short *y_vel_dn;
unsigned short *z_vel_dn;
unsigned short *xr_vel_dn;
unsigned short *sel_cos;
unsigned short *sel_sin;
unsigned short *adc_busy;

char hour,minute,julday,month,aday,year;
int temp[6],z_vel_total,xr_vel_total;

short *read;
unsigned short temp_sel[6];
int delta;
char *units;
float scale;
int i, j, sem_status, temp_cnt;

void mini_rt_isr();

rims_d->hndshk.mailbox = (unsigned int *)MAILBOX;
rims_d->hndshk.intr_flag = 0;    /* default to no interrupt sent */

/****************** INITIALIZE POINTERS *********************/

rd_tmp=(unsigned short *)(MIN_BASE+READ_TEMP);
min_stat=(unsigned short *)(MIN_BASE+MIN_STATUS);
adc_busy=(unsigned short *)(MIN_BASE+ADC_STAT);
x_accel=(unsigned short *)(MIN_BASE+X_AXIS_ACCEL);
y_accel=(unsigned short *)(MIN_BASE+Y_AXIS_ACCEL);
z_accel=(unsigned short *)(MIN_BASE+Z_AXIS_ACCEL);
x_vel_up=(unsigned short *)(MIN_BASE+X_AXIS_VEL_UP);
y_vel_up=(unsigned short *)(MIN_BASE+Y_AXIS_VEL_UP);
z_vel_up=(unsigned short *)(MIN_BASE+Z_AXIS_VEL_UP);
xr_vel_up=(unsigned short *)(MIN_BASE+XR_AXIS_VEL_UP);
x_vel_dn=(unsigned short *)(MIN_BASE+X_AXIS_VEL_DN);
y_vel_dn=(unsigned short *)(MIN_BASE+Y_AXIS_VEL_DN);
z_vel_dn=(unsigned short *)(MIN_BASE+Z_AXIS_VEL_DN);
xr_vel_dn=(unsigned short *)(MIN_BASE+XR_AXIS_VEL_DN);
sel_cos=(unsigned short *)(MIN_BASE+SEL_COS);
sel_sin=(unsigned short *)(MIN_BASE+SEL_SIN);

/***********************************************************/

/* initialize Minirims data structure */

for(i=0;i<=1;i++)
  {
    rims_d->RIMS_data[i].minirims_data_id[0] = 'M';
    rims_d->RIMS_data[i].minirims_data_id[1] = 'I';
    rims_d->RIMS_data[i].minirims_data_id[2] = 'N';
    rims_d->RIMS_data[i].minirims_data_id[3] = 'I';
    rims_d->RIMS_data[i].minirims_data_len = sizeof(minirims_data);
    for (j=0;j<128;j++)
      {
	rims_d->RIMS_data[i].x_axis_acc[j] = 1;	
	rims_d->RIMS_data[i].y_axis_acc[j] = 2;	
	rims_d->RIMS_data[i].z_axis_acc[j] = 3;
	rims_d->RIMS_data[i].x_axis_gyro_up[j] = 4;	
	rims_d->RIMS_data[i].x_axis_gyro_dn[j] = 5;
	rims_d->RIMS_data[i].y_axis_gyro_up[j] = 6;		
	rims_d->RIMS_data[i].y_axis_gyro_dn[j] = 7;
	rims_d->RIMS_data[i].z_axis_gyro_up[j] = 8;			       
	rims_d->RIMS_data[i].z_axis_gyro_dn[j] = 9;
	rims_d->RIMS_data[i].xr_axis_gyro_up[j] = 10;
	rims_d->RIMS_data[i].xr_axis_gyro_dn[j] = 11;			
      }
  }
#ifndef INTERRUPT

/* set up auxilliary clock for 128Hz update rate */

sysAuxClkRateSet(128);  /* was 64 Hz */

/* Connect auxilliary clock interrupt */

if(sysAuxClkConnect((FUNCPTR)(mini_rt_isr),0) == ERROR)
  exit(2);

#endif

/* Create semaphore */

mini_rd_sem = semBCreate(SEM_Q_FIFO,SEM_EMPTY);

#ifdef INTERRUPT

/* Setup Interrupt Vector Table in Host */

if (intConnect((VOIDFUNCPTR *)(int_vec2 * 4), (VOIDFUNCPTR)mini_rt_isr,0) == ERROR)
  {
    fprintf(stderr, "failed to connect interrupt vector %d. Exiting.\n",int_vec2);
    exit(1);
  }
#endif


#ifndef STAND_ALONE

for(;;)
  {

/* reset counters */

mini_init();

for(i=0; i<6; i++)
    {
        temp[i] = 0;            /* zero temperature array */
    }
count = 1;

/* Initialize temperature selections */

temp_sel[0] = X_Y_GYRO_TEMP;
temp_sel[1] = XR_Z_GYRO_TEMP;
temp_sel[2] = X_ACCL_TEMP;
temp_sel[3] = Y_ACCL_TEMP;
temp_sel[4] = Z_ACCL_TEMP;
temp_sel[5] = PLATFORM_TEMP;

/* Variable initialization */

temp_cnt = 0;

/* Wait for Start Semaphore */
    sem_status = semTake(main_sem,WAIT_FOREVER);
    if(sem_status == OK)
      {

#endif /* STAND_ALONE */

	Stop = 0;
	i = 0;
	j = 0;

#ifdef INTERRUPT

/* Program Interrupter Control and Vector Registers */

	intsel = (unsigned short *)(MIN_BASE + CNTRL_REG);
	*intsel = 0x1a;
	intsel = (unsigned short *)(MIN_BASE + VEC_REG);
	*intsel = int_vec2;


/* Enable 128 Hz interrupt from minirims */

	sysIntEnable(2);
#endif

#ifndef INTERRUPT
/* Enable auxilliary clock interrupt */

        sysAuxClkEnable();
#endif

	/* Uncage Minirims and Start Filling data structures */

	Cmd = (RUN_GYROS + X_Y_GYRO_TEMP + GIMB_ENBL) & 0x1ff; /* must pick valid temp sensor ! */
	*Command = Cmd;
	while(Align)
	  {
	    /* Wait for minirims Semaphore */
	    
	        sem_status = semTake(mini_rd_sem,WAIT_FOREVER);
		if(sem_status == OK)
		  {
#ifdef TIME
		    if(count == 1)
		      get_time(&hour,&minute,&rims_d->RIMS_data[i].second,&rims_d->RIMS_data[i].millisecond,&julday,&month,&aday,&year);

#endif
		    delta = *x_accel & 0xfff;
		    if (delta > MAX_CNT)
		      delta = delta - 4096;		    
		    rims_d->RIMS_data[i].x_axis_acc[j] = delta;
		    delta = *y_accel & 0xfff;
		    if (delta > MAX_CNT)
		      delta = delta - 4096;		    
		    rims_d->RIMS_data[i].y_axis_acc[j] = delta;
		    delta = *z_accel & 0xfff;
		    if (delta > MAX_CNT)
		      delta = delta - 4096;		    
		    rims_d->RIMS_data[i].z_axis_acc[j] = -delta;        /* correct for sign */
		    
		    rims_d->RIMS_data[i].x_axis_gyro_up[j] = *x_vel_up & 0xfff;
		    rims_d->RIMS_data[i].x_axis_gyro_dn[j] = -(*x_vel_dn & 0xfff);
		    
		    rims_d->RIMS_data[i].y_axis_gyro_up[j] = -(*y_vel_up & 0xfff); /* correct for sign */
		    rims_d->RIMS_data[i].y_axis_gyro_dn[j] = *y_vel_dn & 0xfff; /* correct for sign */

		    
		    rims_d->RIMS_data[i].z_axis_gyro_up[j] = -(*z_vel_up & 0xfff);  /* correct for sign */
		    rims_d->RIMS_data[i].z_axis_gyro_dn[j] = *z_vel_dn & 0xfff; /* correct for sign */
		    
		    rims_d->RIMS_data[i].xr_axis_gyro_up[j] = -(*xr_vel_up & 0xfff); /* correct for sign */
		    rims_d->RIMS_data[i].xr_axis_gyro_dn[j] = *xr_vel_dn & 0xfff; /* correct for sign */

		  /* Add new resolver Code Here			
		printw("%8.1f",(float)(*rdc * RES_SCALE)); */ 

		    temp[0] += *rd_tmp & 0xfff;

/***************************************************************************/
/*                      One Second Updates                                 */
/***************************************************************************/

		    if(count == 128)
		      {
                        count = 1;
			j = 0;

#ifdef TEMP			
                        /* Cycle thru 6 temperature sensors and update
                           each at 1 sec rate
                         */
                        Cmd |= TEMP_MASK;
                        Cmd &= (temp_sel[temp_cnt] + 0x18F);
                        *Command = Cmd;
			rims_d->RIMS_data[i].temperature = (float)(temp[0]*TEMP_SCALE);
			rims_d->RIMS_data[i].command = Cmd;
                        if(temp_cnt == 5)
                          temp_cnt = 0;
                        else
                          temp_cnt++;
#endif
                        /* Read status bits and update status */

                        rims_d->RIMS_data[i].status = *min_stat & 0xF;
                        
                        temp[0] = 0;
			rims_d->hndshk.status = 0x0;   /* good status */
			rims_d->hndshk.data_flag[i] = 0x1; /* valid data */
			if(rims_d->hndshk.intr_flag == 1)
			  {
			    *(rims_d->hndshk.mailbox) = 0x1;   /* write to Mailbox */
			  }
			i = !i;
			if(rims_d->hndshk.data_flag[i])
			  logMsg("ERROR: Overwriting Unread Data!\n",0,0,0,0,0,0);
		      }
		    else
		      {
			count++;
			j++;
		      }
		    
		  }
	  }
	/* Disable auxilliary clock interrupt */

	sysAuxClkDisable();
	printf("WARNING: Must Align Minirims Before Starting to get Minirims Data!!!\n");
	semTake(main_sem,NO_WAIT); /* take start semaphore just in case got multiple starts */

#ifndef STAND_ALONE
      }
  }
#endif
	
}

void mini_rt_isr()
     
{
  intsel = (unsigned short *)(MIN_BASE + CNTRL_REG);
  *intsel = 0x1a;           /* re-enable interrupt */
  semGive(mini_rd_sem);
}




