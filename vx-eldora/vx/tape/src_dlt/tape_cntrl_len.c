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
 * Revision 1.1  1996/06/18  16:08:00  craig
 * Initial revision
 *
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
len = len + temp * (LAST_CIP_CMD + 1);

temp = sizeof(struct mode_struct);
if(((int)(temp /4) * 4) != temp) temp = ((int)(temp /4) + 1) * 4;
len += temp;

temp = sizeof(struct mode_struct);
if(((int)(temp /4) * 4) != temp) temp = ((int)(temp /4) + 1) * 4;
len += temp;

temp = sizeof(struct request_sense);
if(((int)(temp /4) * 4) != temp) temp = ((int)(temp /4) + 1) * 4;
len += temp;

temp = sizeof(struct log_pg);
if(((int)(temp /4) * 4) != temp) temp = ((int)(temp /4) + 1) * 4;
len += temp;

temp = sizeof(struct supported_pgs);
if(((int)(temp /4) * 4) != temp) temp = ((int)(temp /4) + 1) * 4;
len += temp;

if(sg_or_blocked == SCATTER_GATHER)
  {
      temp = sizeof(struct scatter_gather);
      if(((int)(temp /4) * 4) != temp) temp = ((int)(temp /4) + 1) * 4;
      len = len + NUM_SG_STRUCTS * temp * (MAX_SG_BLK + 2);
  }

return(len);

}
