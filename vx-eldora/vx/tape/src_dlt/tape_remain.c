/*
 *	$Id$
 *
 *	Module: tape_remain		 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1996/06/18  16:07:56  craig
 * Initial revision
 *
 *
 *
 * description:  Returns the percent of tape remaining to be recorded.
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include <tapeDef.h>
#include <tapeGbl.h>

/******************** CHANNEL ATTENTION **********************/

int tape_remain(unsigned char drv_id)
{

int percent, i, index;

union
  {
      unsigned long sense_data;
      unsigned char tape[4];
  }tp;

dlt_cmds(REQUEST_SENSE,drv_id);

index = -1;
for(i=0; i<4; i++)
  if(scsi_id[i] == drv_id) index = i;

if(index == -1) return(100);

percent = ((float)tp.sense_data/
	    (float)starting_amt[index])*100;


return(percent);
}
