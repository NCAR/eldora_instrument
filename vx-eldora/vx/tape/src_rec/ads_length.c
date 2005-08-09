/*
 *	$Id$
 *
 *	Module: ads_length		 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1996/06/24  23:00:08  craig
 * Initial revision
 *
 *
 * description:  Calculates the length of an ADS record to record
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include <recordIncs.h>
#include <tapeDef.h>
#include <tapeGbl.h>

unsigned int ads_length(unsigned int *ads_addr)
{

INSITU_DATA *ads;
unsigned int ads_len;

(unsigned int)ads = *ads_addr;
ads_len = ads->insitu_data_len;
return(ads_len);

}


