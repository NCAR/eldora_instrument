/*

 *	$Id$
 *
 *	Module:	pos.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/08/14  21:34:34  reif
 * Initial revision
 *
 *
 * description: The purpose of this module is to read the position from the 2016
 *              chip and return this floating point value to the calling routine.
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
float count;
short i;
union
  {
      unsigned short degrees;
      unsigned char degr[2];
  }position;

*ltch=0x0C; /* Latch 2016OE* */
for(i=0; i<1; i++);
position.degr[0]=*rd2016; /* Read high byte */
position.degr[1]=*rd2016; /* Read low byte */
*ltch=0x0E; /* Release 2016OE* */
count=(((float)position.degrees)/COUNTS)*360;/* Convert to degrees */
return(count);
}
