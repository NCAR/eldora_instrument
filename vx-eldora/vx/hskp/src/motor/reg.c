/*

 *	$Id$
 *
 *	Module:	motor.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description:
 *             
 *             
 *              
 *              
 *             
 *             
 *             
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include <cntrlIncl.h>

void write_reg()
{
unsigned char *mtr_cmnd;
unsigned char *prog_cntr;
unsigned char period;

prog_cntr=(unsigned char *)PROGRAM_COUNTER;
mtr_cmnd=(unsigned char *)EIGHT_BIT_MOTOR_COMMAND;
*prog_cntr=0x01;
taskDelay(1);
for(;;)
{
    for(period=0x00;period<0xFF; period++)
      {
	  *mtr_cmnd=period;
	  taskDelay(1);
	  printf("MOTOR SHOULD= %4X  MOTOR IS= %4X\r",period,*mtr_cmnd);
	  taskDelay(10 );
      }
}

}
