/*
 *	$Id$
 *
 *	Module:		 
 *	Original Author: 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1994/01/06  21:31:52  craig
 * Initial revision
 *
 * Revision 1.4  1992/10/25  17:09:25  reif
 * *** empty log message ***
 *
 * Revision 1.3  1992/10/17  21:34:37  reif
 * *** empty log message ***
 *
 * Revision 1.2  1992/10/15  17:53:02  reif
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/14  17:53:17  reif
 * Initial revision
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#include <cipincl.h>

unsigned int ads_length(unsigned int *ads_addr)
{
INSITU_DATA *ads;
unsigned int ads_length;

(unsigned int)ads=*ads_addr;
ads_length=ads->insitu_data_len;
return(ads_length);
}


