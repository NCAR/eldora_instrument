/*
 *
 *	$id: $
 *
 *	Module:	test_pos.c	 
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
 *              position in degrees. It also allows the user to experiment
 *              with the Gain, Pole, Zero, and sample rate of the compensator.
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

int test_pos()
{
  float curr_pos, delta,new_pos,pole,zero,gain,sample_rate,temp_pos;
  float prev_pos;
  int choice,status, posit_count;
  short accel;
  unsigned char max_vel;
  union
  {
      int posit;
      unsigned char pos[4];
  }actl;

  init_motor();     /* Initialize Controller */
  set_vel(10.0);    /* Program desired velocity to 10 RPM */
  go_motor();       /* Spin Rotodome */
  taskDelay(1000);  /* Wait approx. 20 seconds -- Rotodome should go thru one full rotation */
  stop_motor();     /* Stop Rotodome */
  curr_pos = get_pos();
  printf("Current Position is: %f Degrees\n",curr_pos);

  /* Initialize Lead Compensator with Default Values */

  gain = 1.0;
  pole = 0.9;
  zero = 0.25;
  new_pos = 90.0;
  max_vel = 0x20;
  accel = 0x2000;   /* Note MSByte is integer part; LSByte is fractional part */
  sample_rate = 2.0e-06;

  while(1)
    {
      printf("POSITION MENU\n");
      printf("\n");
      printf("0) EXIT\n");
      printf("1) SELECT GAIN\n");
      printf("2) SELECT POLE\n");
      printf("3) SELECT ZERO\n");
      printf("4) SELECT MAXIMUM VELOCITY (trapazoidal only)\n");
      printf("5) SELECT ACCELERATION (trapazoidal only)\n");
      printf("6) SELECT SAMPLE RATE in Seconds\n");
      printf("7) SELECT NEW POSITION in Degrees\n");
      printf("8) GOTO POSITION\n");
      printf("9) GOTO POSITION w/TRAPAZOIDAL PROFILE\n");
      printf("10) LIST CURRENT SELECTIONS\n");
      
      scanf(" %d",&choice);
      switch(choice)
	{
	case 0:
	  return(-1);
	  break;
	case 1:
	  printf("1) ENTER GAIN (float)\n");
	  scanf(" %f",&gain);
	  break;
        case 2:
	  do
	    {
	      printf("2) ENTER POLE (float 0<B<1.0)\n");
	      scanf(" %f",&pole);
	      if(pole < 0 || pole > 1.0)
		{
		  status = 1;
		  printf("INVALID POLE ENTRY!\n");
		}
	      else
		status = 0;
	    }while(status);
	  break;
        case 3:
	  do
	    {
	      printf("3) ENTER ZERO (float 0<A<1.0)\n");
	      scanf(" %f",&zero);
	      if(zero < 0 || zero > 1.0)
		{
		  status = 1;
		  printf("INVALID ZERO ENTRY!\n");
		}
	      else
		status = 0;
	    }while(status);
	      break;
        case 4:
	  do
	    {
	      printf("4) ENTER MAXIMUM VELOCITY ( 0<V<127)\n");
	      scanf(" %d",&max_vel);
	      if(max_vel < 0 || max_vel > 127)
		{
		  status = 1;
		  printf("INVALID VELOCITY ENTRY!\n");
		}
	      else
		status = 0;
	    }while(status);
	      break;
        case 5:
	  do
	    {
	      printf("3) ENTER ACCELERATION (0<A<32767)\n");
	      scanf(" %d",&accel);
	      if(accel < 0 || accel > 32767)
		{
		  status = 1;
		  printf("INVALID ACCELERATION ENTRY!\n");
		}
	      else
		status = 0;
	    }while(status);
	      break;
        case 6:
	  do
	    {
	      printf("4) ENTER SAMPLE RATE in Seconds\n");
	      scanf(" %f",&sample_rate);
	      if(sample_rate < 64.0e-06 || sample_rate > 2.048e-03)
		{
		  status = 1;
		  printf("INVALID SAMPLE RATE!\n");
		}
	      else
		status = 0;
	    }while(status);
	  break;
        case 7:
	  do
	    {
	      printf("5) ENTER NEW POSITION in Degrees\n");
	      scanf(" %f",&new_pos);
	      if(new_pos < 0 || new_pos > 360.0)
		{
		  status = 1;
		  printf("INVALID POSITION!\n");
		}
	      else
		status = 0;
	    }while(status);
	      break;
	case 8:
	  do
	    {
	      prev_pos = curr_pos;
	      curr_pos = get_pos();
	    }while(curr_pos != prev_pos);
	  printf("Current Position = %f\n",curr_pos);
	  delta = new_pos - curr_pos;
	  if(delta > 180.0)
	    delta = delta - 360.0;
	  if(delta < -180.0)
	    delta = delta + 360.0;
	  delta = delta * 65536.0/360.0; /* delta in counts */
	  actl.pos[2] = *(unsigned char *)(READ_POSITION_LSB);
	  actl.pos[1] = *(unsigned char *)(READ_POSITION_MID);
	  actl.pos[0] = *(unsigned char *)(READ_POSITION_MSB);
	  posit_count = (actl.posit >> 8) + (int)delta;
	  printf("Current_Position = %f; Delta = %f; Posit_count = %d; Actual Position = %d\n",curr_pos,delta,posit_count,actl.posit >> 8);
	  goto_pos(gain,zero,pole,sample_rate,posit_count);
	  go_motor();
	  do
	    {
	      taskDelay(50);
	      curr_pos = get_pos();
	      actl.pos[2] = *(unsigned char *)(READ_POSITION_LSB);
	      actl.pos[1] = *(unsigned char *)(READ_POSITION_MID);
	      actl.pos[0] = *(unsigned char *)(READ_POSITION_MSB);
	      temp_pos = (float)(actl.posit >> 8);
	      printf("Actual Position is: %f Counts\r",temp_pos);
	      /*	      printf("Current Position is: %f Degrees\r",curr_pos); */
	    }while(fabs(new_pos-curr_pos) > (double)EPSILON);
	  break;
	case 9:
	  curr_pos = get_pos();
	  delta = new_pos - curr_pos;
	  if(delta > 180.0)
	    delta = delta - 360.0;
	  if(delta < -180.0)
	    delta = delta + 360.0;
	  delta = delta * 65536.0/360.0; /* delta in counts */
	  actl.pos[2] = *(unsigned char *)(READ_POSITION_LSB);
	  actl.pos[1] = *(unsigned char *)(READ_POSITION_MID);
	  actl.pos[0] = *(unsigned char *)(READ_POSITION_MSB);
	  posit_count = (actl.posit >> 8) + (int)delta;
	  printf("Current_Position = %f; Delta = %f; Posit_count = %d; Actual Position = %d\n",curr_pos,delta,posit_count,actl.posit >> 8);
	  trap_pos(gain,zero,pole,sample_rate,max_vel,accel,posit_count);
	  go_motor();
	  do
	    {
	      taskDelay(50);
	      curr_pos = get_pos();
	      actl.pos[2] = *(unsigned char *)(READ_POSITION_LSB);
	      actl.pos[1] = *(unsigned char *)(READ_POSITION_MID);
	      actl.pos[0] = *(unsigned char *)(READ_POSITION_MSB);
	      temp_pos = (float)(actl.posit >> 8);
	      printf("Actual Position is: %f Counts\r",temp_pos);
	      /*	      printf("Current Position is: %f Degrees\r",curr_pos); */
	    }while(fabs(new_pos-curr_pos) > (double)EPSILON);
	  break;
	case 10:
	  printf("GAIN = %4f\n",gain);
	  printf("POLE = %4f\n",pole);
	  printf("ZERO = %4f\n",zero);
	  printf("SAMPLE RATE = %4f Seconds\n",sample_rate);
	  printf("DESIRED POSITION = %4f Degrees\n",new_pos);
	  break;

	}
    }
}
