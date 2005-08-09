/*
 *	$Id$
 *
 *	Module: mini_rd.c		 
 *	Original Author: Eric Loew 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1996/02/09  18:29:46  craig
 * Initial revision
 *
 *
 * description:
 *
 * This module reads minirims VME Card counters at a pre-programmed
 * rate (card firmware) and updates the minirims status display.
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define NO_MIN_SCOPE
#define STAND_ALONE
#define TEMP
#define INTERRUPT
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
void mini_rd()
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

int x_accel_cum, y_accel_cum, z_accel_cum, x_vel_cum, y_vel_cum;
int z_vel_cum, xr_vel_cum;
int x_accel_total, y_accel_total, z_accel_total, x_vel_total, y_vel_total;
int temp[6],z_vel_total,xr_vel_total;
unsigned short x_accel_prev, y_accel_prev, z_accel_prev;
unsigned short x_vel_prev, y_vel_prev, z_vel_prev, xr_vel_prev;
short *read;
unsigned short temp_sel[6];
int delta;
char *units;
float scale;
int i, sem_status, temp_cnt, t_cnt;

void mini_isr();

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

#ifndef INTERRUPT

/* set up auxilliary clock for 128Hz update rate */

sysAuxClkRateSet(128);  /* was 64 Hz */

/* Connect auxilliary clock interrupt */

if(sysAuxClkConnect((FUNCPTR)(mini_isr),0) == ERROR)
  exit(2);

#endif

/* Create semaphore */

mini_rd_sem = semBCreate(SEM_Q_FIFO,SEM_EMPTY);

#ifdef INTERRUPT

/* Setup Interrupt Vector Table in Host */

if (intConnect((VOIDFUNCPTR *)(int_vec2 * 4), (VOIDFUNCPTR)mini_isr,0) == ERROR)
  {
    fprintf(stderr, "failed to connect interrupt vector %d. Exiting.\n",int_vec2);
    exit(1);
  }
#endif

/* reset counters */

mini_init();

Cmd = STBY; /* put minirims in STANDBY */
*Command = Cmd;
for(i=0; i<6; i++)
    {
        temp[i] = 0;            /* zero temperature array */
    }
count = 1;
t_cnt = 1;

/* Initialize temperature selections */

temp_sel[0] = X_Y_GYRO_TEMP;
temp_sel[1] = XR_Z_GYRO_TEMP;
temp_sel[2] = X_ACCL_TEMP;
temp_sel[3] = Y_ACCL_TEMP;
temp_sel[4] = Z_ACCL_TEMP;
temp_sel[5] = PLATFORM_TEMP;

/* Variable initialization */

temp_cnt = 0;
x_accel_prev = 0;
y_accel_prev = 0;
z_accel_prev = 0;
x_vel_prev = 0;
y_vel_prev = 0;
z_vel_prev = 0;
xr_vel_prev = 0;
x_accel_cum = 0;
y_accel_cum = 0;
z_accel_cum = 0;
x_vel_cum = 0;
y_vel_cum = 0;
z_vel_cum = 0;
xr_vel_cum = 0;
x_accel_total = 0;
y_accel_total = 0;
z_accel_total = 0;
x_vel_total = 0;
y_vel_total = 0;
z_vel_total = 0;
xr_vel_total = 0;

#ifndef STAND_ALONE

for(;;)
  {

/* Wait for Start Semaphore */
    sem_status = semTake(main_sem,WAIT_FOREVER);
    if(sem_status == OK)
      {

#endif /* STAND_ALONE */

	Stop = 0;

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

	while(cmd != 12 && !Stop)
	  {
/* Wait for minirims Semaphore */

		sem_status = semTake(mini_rd_sem,WAIT_FOREVER);
		if(sem_status == OK)
		  {
		    delta = *x_accel & 0xfff;
		    if (delta > MAX_CNT)
		      delta = delta - 4096;		    
		    x_accel_cum += delta;
		    delta = *y_accel & 0xfff;
		    if (delta > MAX_CNT)
		      delta = delta - 4096;		    
		    y_accel_cum += delta;
		    delta = *z_accel & 0xfff;
		    if (delta > MAX_CNT)
		      delta = delta - 4096;		    
		    z_accel_cum -= delta;        /* correct for sign */
		    
		    delta = (*x_vel_up & 0xfff) - (*x_vel_dn & 0xfff);
		    x_vel_cum += delta;
		    
		    delta = (*y_vel_up & 0xfff) - (*y_vel_dn & 0xfff);
		    y_vel_cum -= delta;          /* correct for sign */
		    
		    delta = (*z_vel_up & 0xfff) - (*z_vel_dn & 0xfff);
		    z_vel_cum -= delta;          /* correct for sign */
		    
		    delta = (*xr_vel_up & 0xfff) - (*xr_vel_dn & 0xfff);
		    xr_vel_cum -= delta;         /* correct for sign */
		    
		    temp[0] += *rd_tmp & 0xfff;
		    if(count == 128)
		      {
                        count = 1;
#ifdef TEMP			
                        /* Cycle thru 6 temperature sensors and update
                           each at 1 sec rate
                         */
                        Cmd |= TEMP_MASK;
                        Cmd &= (temp_sel[temp_cnt] + 0x8F);
                        *Command = Cmd;
                        switch(temp_cnt)
			  {
			  case 0:
			    strcpy(dp->temp_df2,"");
			    mvaddstr(TEMP_FH2_Y,70,dp->temp_df2);
			    printw("%6.1f C",(float)(temp[0]*TEMP_SCALE)); 
			    break;
			  case 1:
			    strcpy(dp->temp_df3,"");
			    mvaddstr(TEMP_FH3_Y,70,dp->temp_df3);
			    printw("%6.1f C",(float)(temp[0]*TEMP_SCALE));
			    break;
			  case 2:
			    strcpy(dp->temp_df4,"");
			    mvaddstr(TEMP_FH4_Y,70,dp->temp_df4);
			    printw("%6.1f C",(float)(temp[0]*TEMP_SCALE));
			    break;
			  case 3:
			    strcpy(dp->temp_df5,"");
			    mvaddstr(TEMP_FH5_Y,70,dp->temp_df5);
			    printw("%6.1f C",(float)(temp[0]*TEMP_SCALE));
			    break;
			  case 4:
			    strcpy(dp->temp_df6,"");
			    mvaddstr(TEMP_FH6_Y,70,dp->temp_df6);
			    printw("%6.1f C",(float)(temp[0]*TEMP_SCALE));
			    break;
			  case 5:
			    strcpy(dp->temp_df8,"");
			    mvaddstr(TEMP_FH8_Y,70,dp->temp_df8);
			    printw("%6.1f C",(float)(temp[0]*TEMP_SCALE));
			    break;
			  }
                        if(temp_cnt == 5)
                          temp_cnt = 0;
                        else
                          temp_cnt++;
#endif
                        /* Read status bits and update status */

                        stat = *min_stat;
                        if(stat & 0x1)
                          strcpy(dp->pltfm_df3,"CLOSED");
                        else
                          strcpy(dp->pltfm_df3,"OPEN  ");
                        mvaddstr(PLTFM_FH1_Y,73,dp->pltfm_df1);
                        if(stat & 0x2)
                          strcpy(dp->pltfm_df2,"CLOSED");
                        else
                          strcpy(dp->pltfm_df2,"OPEN  ");
                        mvaddstr(PLTFM_FH2_Y,73,dp->pltfm_df2);
                        if(stat & 0x4)
                          strcpy(dp->pltfm_df1,"ON ");
                        else
                          strcpy(dp->pltfm_df1,"OFF");
                        mvaddstr(PLTFM_FH3_Y,73,dp->pltfm_df3);
                        if(stat & 0x8)
                          strcpy(dp->pltfm_df4,"GOOD");
                        else
                          strcpy(dp->pltfm_df4,"LOW ");
                        mvaddstr(PLTFM_FH4_Y,73,dp->pltfm_df4);
                        
                        /* Update accelerometer and gyro counts at 1 sec rate */
                        strcpy(dp->acc_gyr_df1a,"");
                        mvaddstr(ACC_GYR_FH1_Y,11,dp->acc_gyr_df1a);
                        printw("%8.1f",(float)(x_accel_cum)); 

                        strcpy(dp->acc_gyr_df2a,"");
                        mvaddstr(ACC_GYR_FH2_Y,11,dp->acc_gyr_df2a);
                        printw("%8.1f",(float)(y_accel_cum)); 

                        strcpy(dp->acc_gyr_df3a,"");
                        mvaddstr(ACC_GYR_FH3_Y,11,dp->acc_gyr_df3a);
                        printw("%8.1f",(float)(z_accel_cum)); 

                        strcpy(dp->acc_gyr_df4a,"");
                        mvaddstr(ACC_GYR_FH4_Y,11,dp->acc_gyr_df4a);
                        printw("%8.1f",(float)(x_vel_cum)); 

                        strcpy(dp->acc_gyr_df5a,"");
                        mvaddstr(ACC_GYR_FH5_Y,11,dp->acc_gyr_df5a);
                        printw("%8.1f",(float)(y_vel_cum)); 

                        strcpy(dp->acc_gyr_df6a,"");
                        mvaddstr(ACC_GYR_FH6_Y,11,dp->acc_gyr_df6a);
                        printw("%8.1f",(float)(z_vel_cum)); 

                        /* Display Resolver Angle */
                        
                        strcpy(dp->resolver_df,"");
                        mvaddstr(RES_Y,11,dp->resolver_df);

			/* Add new resolver Code Here			
                        printw("%8.1f",(float)(*rdc * RES_SCALE)); */ 

                        refresh();
			

/*
  printf("%5.1f  %5.1f  %5.1f  %5.1f  %5.1f %5.1f %5.1f  %5.1fC\n",(float)(x_accel_cum),(float)(y_accel_cum),(float)(z_accel_cum),(float)(x_vel_cum),(float)(y_vel_cum),(float)(z_vel_cum),(float)(xr_vel_cum),(float)(temp[0]*TEMP_SCALE)); */

                        x_accel_total += x_accel_cum;
                        y_accel_total += y_accel_cum;
                        z_accel_total += z_accel_cum;
                        x_vel_total += x_vel_cum;
                        y_vel_total += y_vel_cum;
                        z_vel_total += z_vel_cum;
                        xr_vel_total += xr_vel_cum;

                        x_accel_cum = 0;
                        y_accel_cum = 0;
                        z_accel_cum = 0;
                        x_vel_cum = 0;
                        y_vel_cum = 0;
                        z_vel_cum = 0;
                        xr_vel_cum = 0;
                        temp[0] = 0;


                        /* Update accelerometer and gyro counts at 30 sec rate */
                        if(t_cnt == 30)
			  {
			      strcpy(dp->acc_gyr_df1b,"");
			      mvaddstr(ACC_GYR_FH1_Y,24,dp->acc_gyr_df1b);
			      printw("%8.1f",(float)(x_accel_total)); 

			      strcpy(dp->acc_gyr_df2b,"");
			      mvaddstr(ACC_GYR_FH2_Y,24,dp->acc_gyr_df2b);
			      printw("%8.1f",(float)(y_accel_total)); 
			      
			      strcpy(dp->acc_gyr_df3b,"");
			      mvaddstr(ACC_GYR_FH3_Y,24,dp->acc_gyr_df3b);
			      printw("%8.1f",(float)(z_accel_total)); 
			      
			      strcpy(dp->acc_gyr_df4b,"");
			      mvaddstr(ACC_GYR_FH4_Y,24,dp->acc_gyr_df4b);
			      printw("%8.1f",(float)(x_vel_total)); 
			      
			      strcpy(dp->acc_gyr_df5b,"");
			      mvaddstr(ACC_GYR_FH5_Y,24,dp->acc_gyr_df5b);
			      printw("%8.1f",(float)(y_vel_total)); 
			      
			      strcpy(dp->acc_gyr_df6b,"");
			      mvaddstr(ACC_GYR_FH6_Y,24,dp->acc_gyr_df6b);
			      printw("%8.1f",(float)(z_vel_total)); 
			      
			      refresh();
			      
			      t_cnt = 1;
			      x_accel_total = 0;
			      y_accel_total = 0;
			      z_accel_total = 0;
			      x_vel_total = 0;
			      y_vel_total = 0;
			      z_vel_total = 0;
			      xr_vel_total = 0;
			  }
                        else
                          t_cnt++;
		      }
		    else
		      count++;
		    
		  }
	  }
	/* Disable auxilliary clock interrupt */

	sysAuxClkDisable();
	semTake(main_sem,NO_WAIT); /* take start semaphore just in case got multiple starts */

#ifndef STAND_ALONE
      }
  }
#endif
	
}

void mini_isr()
     
{
  intsel = (unsigned short *)(MIN_BASE + CNTRL_REG);
  *intsel = 0x1a;           /* re-enable interrupt */
  semGive(mini_rd_sem);
}




