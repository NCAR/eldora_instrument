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


