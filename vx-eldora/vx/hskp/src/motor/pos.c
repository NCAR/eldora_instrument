/*

 *	$Id$
 *
 *	Module:	cntrl_Def.h	 
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


float get_pos()
{
unsigned char *ltch,*rd2016;
float count;
short i;
union
  {
      unsigned short degrees;
      unsigned char degr[2];
  }position;

ltch=(unsigned char *)CONTROL_LATCH;
rd2016=(unsigned char *)READ_2016;
for(;;)
  {
      *ltch=0x0C; /* Latch 2016OE* */
      taskDelay(1);
      position.degr[0]=*rd2016; /* Read high byte */
      taskDelay(1);
      position.degr[1]=*rd2016; /* Read low byte */
      taskDelay(1);
      *ltch=0x0E;
      count=(((float)position.degrees)/COUNTS)*360;
      printf("HI=%04X LO=%04X DEG=%3.2f\r",position.degr[0],position.degr[1],count);
      taskDelay(10);
  }
return(count);
}
