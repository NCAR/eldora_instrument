/*

 *	$Id$
 *
 *	Module:	get_pos.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1992/09/03  15:56:16  craig
 * *** empty log message ***
 *
 * Revision 1.1  1992/09/01  20:42:14  craig
 * Initial revision
 *
 *
 * description: The purpose of this module is to read the position from the
 *              2016 chip convert it to degrees and return this floating
 *              point value to the calling routine.
 *             
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern

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

float get_pos()
{
float count;
long temp;
short i;
union
  {
      unsigned short degrees;
      unsigned char degr[2];
  }position;

*mot_ltch=0x0C;                   /* Latch 2016OE* */

for(i=0; i<1; i++);               /* Waste a little time */

position.degr[0]=*rd2016;          /* Read high byte */
position.degr[1]=*rd2016;          /* Read low byte */

*mot_ltch=0x0E;                   /* Release 2016OE* */

temp = position.degrees;
temp = temp & 0x0000FFFF;
count= (float)temp/cntrlCounts*360.0;      /* Convert to degrees */

return(count);
}
