/*
 *	$Id$
 *
 *	Module:	sync.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  2003/10/02  15:29:45  kapoor
 * *** empty log message ***
 *
 * Revision 1.1  2003/10/01  20:18:14  kapoor
 * Initial revision
 *
 * Revision 1.4  1994/05/20  20:36:45  craig
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/14  21:42:52  reif
 * Initial revision
 *
 *
 * description: The purpose of this module is to sync the time-of-day (tod)
 *              board's clock chip to the incomming IRIG-B signal coming
 *              from ADS (or any other source).
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define scope extern
#include "hskpAll.h"

char sync_irig() 
{

  /* This function is going to do nothing, because, 
     this function has nothing to do.
  *******/

return((char)1);
}







