/*
 *	$Id$
 *
 *	Module: tape_cntrl_len		 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 *
 * description:  Calculates size of the tape control area needed.
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include <tapeDef.h>
#include <tapeGbl.h>

/******************** CHANNEL ATTENTION **********************/

int tape_cntrl_len(int sg_or_blocked)
{
int i, len, temp;
len = 4;

temp = sizeof(struct param_block);
if(((int)(temp /4) * 4) != temp) temp = ((int)(temp /4) + 1) * 4;
len = len + temp * (LAST_PARAM_BLOCK + 1);

temp = sizeof(struct mode_select);
if(((int)(temp /4) * 4) != temp) temp = ((int)(temp /4) + 1) * 4;
len += temp;

temp = sizeof(struct mode_sense);
if(((int)(temp /4) * 4) != temp) temp = ((int)(temp /4) + 1) * 4;
len += temp;

temp = sizeof(struct request_sense);
if(((int)(temp /4) * 4) != temp) temp = ((int)(temp /4) + 1) * 4;
len += temp;

if(sg_or_blocked == SCATTER_GATHER)
  {
      temp = sizeof(struct scatter_gather);
      if(((int)(temp /4) * 4) != temp) temp = ((int)(temp /4) + 1) * 4;
      len = len + 2 * temp * (MAX_SG_BLK + 2);
  }

return(len);

}
