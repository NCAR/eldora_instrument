/*
 *
 *	$id: $
 *
 *	Module:	pos_loop.c	 
 *	Original Author: E. Loew 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * 
 * 
 * Initial revision
 *
 * description: This function will cause the motor to goto a user specified
 *              position in degrees. PID control is implimented and the user
 *              can specify Kp, Kd, Kt, and the sample rate.
 *             
 *             
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define scope extern
#define EPSILON 0.01

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


/* include the .h files that are housekeeper code specific */

#include "tp41vAdr.h"
#include "cntrlDef.h"
#include "cntrlGbl.h"
#include "cntrlFunc.h"
#ifdef good
#define Kp 1.0*(360*16/65536.0)*12.8   /* 1 amp/degree */
#define Kd 1.0*(360*16/65536.0)*12.8   /* 1 amp/degree/sec */
#define Ki 0.25*(360*16/65536.0)*12.8  /* 0.25 amps/sec/degree */
#endif
#ifndef good
#define Kp 35.0
#define Kd  8.0
#define Ki 25.0
#endif
#define S_RATE 0.025
#define SCALE_FAC 1.0
#define MAX_POS_VOLTS 111.0
#define MAX_NEG_VOLTS -112.0
#define MAX_POS_SUM 63.0
#define MAX_NEG_SUM -64.0
#define MAX_POS_PROP_ERR 511.0
#define MAX_NEG_PROP_ERR -512.0


int pos_loop()
{
  float curr_pos, delta,cmnd_pos,sample_rate,temp_pos,prev_delta;
  float prev_pos, prop_gain, diff_gain, integ_gain, servo_cmnd;
  float sum_delta, prop_err;
  int choice,status, posit_count, count;
  register int delay;

  init_motor();     /* Initialize Controller */
  set_vel(10.0);    /* Program desired velocity to 10 RPM */
  go_motor();       /* Spin Rotodome */
  taskDelay(250);  /* Wait approx. 5 seconds -- Rotodome should go thru one full rotation */
  stop_motor();     /* Stop Rotodome */
  curr_pos = get_pos();
  printf("Current Position is: %f Degrees\n",curr_pos);

  /* Initialize PID Controller with defaults */

  prop_gain = Kp;
  diff_gain = Kd;
  integ_gain = Ki;
  sample_rate = S_RATE;
  cmnd_pos = 90.0;

  while(1)
    {
      printf("POSITION MENU\n");
      printf("\n");
      printf("0) EXIT\n");
      printf("1) SELECT PROPORTIONAL GAIN\n");
      printf("2) SELECT DIFFERENTIAL GAIN\n");
      printf("3) SELECT INTEGRAL GAIN\n");
      printf("4) SELECT SAMPLE RATE in Seconds\n");
      printf("5) SELECT NEW POSITION in Degrees\n");
      printf("6) GOTO POSITION\n");
      printf("7) LIST CURRENT SELECTIONS\n");
      
      scanf(" %d",&choice);
      switch(choice)
	{
	case 0:
	  return(-1);
	  break;
	case 1:
	  printf("1) ENTER PROPORTIONAL GAIN (float)\n");
	  scanf(" %f",&prop_gain);
	  break;
        case 2:
	      printf("2) ENTER DIFFERENTIAL GAIN (float)\n");
	      scanf(" %f",&diff_gain);
	  break;
        case 3:
	      printf("3) ENTER INTEGRAL GAIN (float)\n");
	      scanf(" %f",&integ_gain);
	      break;
        case 4:
	  do
	    {
	      printf("4) ENTER SAMPLE RATE in Seconds (float)\n");
	      scanf(" %f",&sample_rate);
	      if(sample_rate < 0.0 || sample_rate > 1.0)
		{
		  status = 1;
		  printf("INVALID SAMPLE RATE!\n");
		}
	      else
		status = 0;
	    }while(status);
	  break;
        case 5:
	  do
	    {
	      printf("5) ENTER NEW POSITION in Degrees\n");
	      scanf(" %f",&cmnd_pos);
	      if(cmnd_pos < 0 || cmnd_pos > 360.0)
		{
		  status = 1;
		  printf("INVALID POSITION!\n");
		}
	      else
		status = 0;
	    }while(status);
	      break;
	case 6:
	  do
	    {
	      prev_pos = curr_pos;
	      curr_pos = get_pos();
	    }while(curr_pos != prev_pos);
	  printf("Current Position = %f\n",curr_pos);
	  delta = cmnd_pos - curr_pos;
	  if(delta > 180.0)
	    delta = delta - 360.0;
	  if(delta < -180.0)
	    delta = delta + 360.0;
	  delta = delta * 65536.0/360.0 * SCALE_FAC; /* delta in counts */
	  prev_delta = delta;
	  sum_delta = 0.0;
	  count = 0;
	  while(count<1200)
	    {
	  prop_err = prop_gain * delta;
	  if(prop_err > MAX_POS_PROP_ERR)
	     prop_err = MAX_POS_PROP_ERR;
	  if(prop_err < MAX_NEG_PROP_ERR)
	     prop_err = MAX_NEG_PROP_ERR;
	  servo_cmnd = prop_err + diff_gain/sample_rate * (delta - prev_delta) + integ_gain * sample_rate * delta + sum_delta;
	  sum_delta = integ_gain * sample_rate * delta + sum_delta;
	  if(sum_delta > MAX_POS_SUM)
	    sum_delta = MAX_POS_SUM;
	  if(sum_delta < MAX_NEG_SUM)
	    sum_delta = MAX_NEG_SUM;
	  prev_delta = delta;
	  if(servo_cmnd > MAX_POS_VOLTS)
	    servo_cmnd = MAX_POS_VOLTS;
	  if(servo_cmnd < MAX_NEG_VOLTS)
	    servo_cmnd = MAX_NEG_VOLTS;
	  *mot_prog_cntr = 0x01;
	  taskDelay(1);
	  *(char *)(EIGHT_BIT_MOTOR_COMMAND) = (char )servo_cmnd + 128;
	  curr_pos = get_pos();
	  delta = cmnd_pos - curr_pos;
	  printf("Current Position is: %f Degrees\r",curr_pos);
	  delay = sample_rate/0.01 + 0.5;
	  taskDelay(delay);
	  count++;
	    }
	  *(char *)(EIGHT_BIT_MOTOR_COMMAND) = 128; /* zero voltage */
	  break;
	case 7:
	  printf("PROPORTIONAL GAIN = %4f\n",prop_gain);
	  printf("DIFFERENTIAL GAIN = %4f\n",diff_gain);
	  printf("INTEGRAL GAIN = %4f\n",integ_gain);
	  printf("SAMPLE RATE = %4f Seconds\n",sample_rate);
	  printf("DESIRED POSITION = %4f Degrees\n",cmnd_pos);
	  break;       	   
	 
	}
    }
}
