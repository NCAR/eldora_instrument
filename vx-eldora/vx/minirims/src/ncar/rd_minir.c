/*
 *	$Id$
 *
 *	Module: rd_minir.c		 
 *	Original Author: Eric Loew 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description:
 *
 * This module reads minirims VME Card counters at a pre-programmed
 * rate and updates the minirims nav structure with accelerometer,
 * gyro, and resolver information.
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define NO_MINIR_SCOPE
#define scope extern
#include "minirGbl.h"
#include "minDef.h"
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include "minGbl.h"
#include "minDisp.h"
#include "string.h"
#include "semLib.h"


/************************* MAIN PROGRAM *********************/
static SEM_ID mini_rd_sem;
static unsigned short *reset;
static unsigned short *ltch_cnt;
static int count;
void rd_minir()
{
register NAV_VARS *n;
unsigned short *rd_tmp;
unsigned short *min_stat,stat;
unsigned short *rdc;
unsigned short *res_stat;
unsigned short *x_accel;
unsigned short *y_accel;
unsigned short *z_accel;
unsigned short *x_vel;
unsigned short *y_vel;
unsigned short *z_vel;
unsigned short *xr_vel;
unsigned short x_accel_prev, y_accel_prev, z_accel_prev;
unsigned short x_vel_prev, y_vel_prev, z_vel_prev, xr_vel_prev;
short *read;
register int delta;
char *units;
float scale;
int i, sem_status, temp_cnt, t_cnt;

void mini_isr();

/****************** INITIALIZE POINTERS *********************/

n = nav_ptr;
reset=(unsigned short *)(MIN_BASE+COUNT_RESET); 
rd_tmp=(unsigned short *)(MIN_BASE+READ_TEMP);
ltch_cnt=(unsigned short *)(MIN_BASE+LATCH_COUNT);
min_stat=(unsigned short *)(MIN_BASE+MIN_STATUS);
rdc=(unsigned short *)(MIN_BASE+RESOLVER);
res_stat=(unsigned short *)(MIN_BASE+RESOLVER_STAT);
x_accel=(unsigned short *)(MIN_BASE+X_AXIS_ACCEL);
y_accel=(unsigned short *)(MIN_BASE+Y_AXIS_ACCEL);
z_accel=(unsigned short *)(MIN_BASE+Z_AXIS_ACCEL);
x_vel=(unsigned short *)(MIN_BASE+X_AXIS_VEL);
y_vel=(unsigned short *)(MIN_BASE+Y_AXIS_VEL);
z_vel=(unsigned short *)(MIN_BASE+Z_AXIS_VEL);
xr_vel=(unsigned short *)(MIN_BASE+XR_AXIS_VEL);

/***********************************************************/
/* set up auxilliary clock for 128Hz update rate */

sysAuxClkRateSet(128);

/* Connect auxilliary clock interrupt */

if(sysAuxClkConnect((FUNCPTR)(mini_isr),0) == ERROR)
  exit(2);

/* Enable auxilliary clock interrupt */

sysAuxClkEnable();

/* Create semaphore */

mini_rd_sem = semBCreate(SEM_Q_FIFO,SEM_EMPTY);

/* reset counters */

mini_init();

/* Variable initialization */

temp_cnt = 0;
x_accel_prev = 0;
y_accel_prev = 0;
z_accel_prev = 0;
x_vel_prev = 0;
y_vel_prev = 0;
z_vel_prev = 0;
xr_vel_prev = 0;

while(1)
    {

        sem_status = semTake(mini_rd_sem,WAIT_FOREVER);  /* Wait for minirims
                                                            Semaphore */
        if(sem_status == OK)
            {

                /* Read Accelerometer counts into data structure */
                
                delta = *x_accel - x_accel_prev;
                if(delta > 512)             /* 512 should be max counts in 1/128 seconds */
                  delta = 65536 - delta;     /* max counts - delta */
                if(delta < -512)
                  delta = -delta -65536;
                n->ixan = delta;
                x_accel_prev = *x_accel;

                delta = *y_accel - y_accel_prev;
                if(delta > 512)             /* 512 should be max counts in 1/128 seconds */
                  delta = 65536 - delta;     /* max counts - delta */
                if(delta < -512)
                  delta = -delta -65536;
                n->iyan = delta;
                y_accel_prev = *y_accel;

                delta = *z_accel - z_accel_prev;
                if(delta > 512)             /* 512 should be max counts in 1/128 seconds */
                  delta = 65536 - delta;     /* max counts - delta */
                if(delta < -512)
                  delta = -delta -65536;
                n->izan = -delta;             /* correct for sign */
                z_accel_prev = *z_accel;

                /* Read Gyro Counts and store in data structure */
                

                delta = *x_vel - x_vel_prev;
                if(delta > 512)             /* 512 should be max counts in 1/128 seconds */
                  delta = 65536 - delta;     /* max counts - delta */
                if(delta < -512)
                  delta = -delta -65536;
                if(delta > 0)                
                    {
                        n->ixgp = delta; 
                        n->ixgn = 0;
                    }
                if(delta < 0)
                    {
                        n->ixgp = 0;
                        n->ixgn = delta;   
                    }
                x_vel_prev = *x_vel;

                delta = *xr_vel - xr_vel_prev;
                if(delta > 512)             /* 512 should be max counts in 1/128 seconds */
                  delta = 65536 - delta;     /* max counts - delta */
                if(delta < -512)
                  delta = -delta -65536;
                if(delta > 0)                
                    {
                        n->ixrgp = -delta;   /* correct for sign */      
                        n->ixrgn = 0;
                    }
                if(delta < 0)
                    {
                        n->ixrgp = 0;
                        n->ixrgn = -delta;   /* correct for sign */
                    }
                xr_vel_prev = *xr_vel;

                delta = *y_vel - y_vel_prev;
                if(delta > 512)             /* 512 should be max counts in 1/128 seconds */
                  delta = 65536 - delta;     /* max counts - delta */
                if(delta < -512)
                  delta = -delta -65536;
                if(delta > 0)
                    {
                        n->iygp = -delta;          /* correct for sign */
                        n->iygn = 0;
                    }
                if(delta < 0)
                    {
                        n->iygp = 0;
                        n->iygn = -delta;          /* correct for sign */
                    }
                y_vel_prev = *y_vel;

                delta = *z_vel - z_vel_prev;
                if(delta > 512)             /* 512 should be max counts in 1/128 seconds */
                  delta = 65536 - delta;     /* max counts - delta */
                if(delta < -512)
                  delta = -delta -65536;
                if(delta > 0)
                    {
                        n->izgp = -delta;          /* correct for sign */
                        n->izgn = 0;
                    }
                if(delta < 0)
                    {
                        n-> izgp = 0;
                        n-> izgn = -delta;         /* correct for sign */
                    }
                z_vel_prev = *z_vel;

            }
    }

}

void mini_isr()

{
    *reset=0x7f;        /* set up counter reset vector */
    *ltch_cnt=0x00;     /* latch counter values and reset selected counters */
    semGive(mini_rd_sem);
    
}




