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
  float curr_pos,new_pos,pole,zero,gain,sample_rate;
  int choice,status;

  init_motor();     /* Initialize Controller */
  set_vel(10.0);    /* Program desired velocity to 10 RPM */
  go_motor();       /* Spin Rotodome */
  taskDelay(2000);  /* Wait approx. 20 seconds -- Rotodome should go thru one full rotation */
  stop_motor();     /* Stop Rotodome */
  curr_pos = get_pos();
  printf("Current Position is: %4F Degrees\n",curr_pos);

  /* Initialize Lead Compensator with Default Values */

  gain = 10.0;
  pole = 0.7;
  zero = 0.4;
  new_pos = 90.0;
  sample_rate = 0.001;

  while(1)
    {
      printf("POSITION MENU\n");
      printf("\n");
      printf("0) EXIT\n");
      printf("1) SELECT GAIN\n");
      printf("2) SELECT POLE\n");
      printf("3) SELECT ZERO\n");
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
	  printf("1) ENTER GAIN (float)\n");
	  scanf(" %f",&gain);
	  break;
        case 2:
      printf("2) ENTER POLE (float 0<B<1.0)\n");
	  scanf(" %f",&pole);
	  break;
        case 3:
      printf("3) ENTER ZERO (float 0<A<1.0)\n");
	  scanf(" %f",&zero);
	  break;
        case 4:
      printf("4) ENTER SAMPLE RATE in Seconds\n");
	  scanf(" %f",&sample_rate);
	  break;
        case 5:
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
	    }while(!status);
	      break;
	case 6:
	  /*	  goto_pos(gain,zero,pole,sample_rate,new_pos); */
	  goto_pos(new_pos);
	  go_motor();
	  do
	    {
	      taskDelay(50);
	      curr_pos = get_pos();
	      printf("Current Position is: %4F Degrees\r",curr_pos);	      
	    }while(fabs(new_pos-curr_pos) > (double)EPSILON);
	  break;
	case 7:
	  printf("GAIN = %4f\n",gain);
	  printf("POLE = %4f\n",pole);
	  printf("ZERO = %4f\n",zero);
	  printf("SAMPLE RATE = %4f Seconds\n",sample_rate);
	  printf("DESIRED POSITION = %4f Degrees\n",new_pos);
	  break;

	}
    }
}
