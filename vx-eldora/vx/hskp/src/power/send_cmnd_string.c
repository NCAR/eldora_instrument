/*
 *	$Id$
 *
 *	Module:	send_cmnd_string.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/09/01  16:48:23  craig
 * Initial revision
 *
 *
 * description: The purpose of this module is to 
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include <vxWorks.h>
#include <stdioLib.h>
#include <math.h>
#include <string.h>
#include <pwrDef.h>
#include <pwrFunc.h>

void send_cmnd_string(char channel,char *string_array)
{
    int i;

    switch(channel)
      {
	case XMIT:
	  for (i=0; i<strlen(string_array); i++) 
	    {
		*g1dido=string_array[i]; /* SEND COMMAND STRING TO PPM ONE */
		taskDelay(2);            /* CHARACTER AT A TIME */
	    }
	  *g1dido='\n';
	  break;
	case TESTP:
	  for (i=0; i<strlen(string_array); i++) 
	    {
		*g2dido=string_array[i]; /* SEND COMMAND STRING TO PPM ONE */
		taskDelay(2);            /* CHARACTER AT A TIME */
	    }
	  *g2dido='\n';
	  break;
	case XMIT_AND_TESTP: 
	  for (i=0; i<strlen(string_array); i++) 
	    {
		*g1dido=string_array[i];
		*g2dido=string_array[i]; /* SEND COMMAND STRING TO PPM ONE */
		taskDelay(2);            /* CHARACTER AT A TIME */
	    }
	  *g1dido='\n';
	  taskDelay(1);
	  *g2dido='\n';
	  break;
      }

}
