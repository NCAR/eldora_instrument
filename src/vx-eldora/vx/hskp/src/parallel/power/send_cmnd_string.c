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
 * Revision 1.2  2000/05/15  18:46:39  eric
 * Added GPIB handshaking and timeout to improve reliability.
 *
 * Revision 1.1  1993/09/20  17:35:55  reif
 * Initial revision
 *
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

int send_cmnd_string(char channel,char *string_array)
{
    int i,timeout,status_f,status_t;
    unsigned char status, status1;
    int MAXWAIT = 30;
    status_t = 0;
    switch(channel)
      {
	case XMIT:
	  for (i=0; i<strlen(string_array); i++) 
	    {
		timeout = 0;
		*g1dido=string_array[i]; /* SEND COMMAND STRING TO PPM ONE */

		/*		taskDelay(2); */ /* CHARACTER AT A TIME */
		while(*g1ism0 != 0x10 && timeout<MAXWAIT)
		  {
		    timeout++;
		    taskDelay(1);
		  }
		if(timeout >= MAXWAIT)
		  status_t = 1;
	    }
	  *g1dido='\n';
	  timeout = 0;
	  while(*g1ism0 != 0x10 && timeout<MAXWAIT)
	    {
	      timeout++;
	      taskDelay(1);
	    }
	  if(timeout >= MAXWAIT)
	    status_t = 1;
	  status = *g1ism1;
	  if(status == 0x2 || status_t == 1)   /* Did 8502 issue SRQ in response to command or did we timeout? */
	    status_f = -1;
	  else
	    status_f = 1;
	  break;
	case TESTP:
	  for (i=0; i<strlen(string_array); i++) 
	    {
	      timeout = 0;
		*g2dido=string_array[i]; /* SEND COMMAND STRING TO PPM ONE */
		/*		taskDelay(2); */ /* CHARACTER AT A TIME */
		while(*g2ism0 != 0x10 && timeout < MAXWAIT)
		  {
		    timeout++;
		    taskDelay(1);
		  }
		/*		printf("Timeout %d\n",timeout); */
		if(timeout >= MAXWAIT)
		  status_t = 1;
	    }
	  timeout = 0;
	  *g2dido='\n';
	  while(*g2ism0 != 0x10 && timeout < MAXWAIT)
	    {
	      timeout++; 
	      taskDelay(1);
	    }
	  if(timeout >= MAXWAIT)
	    status_t = 1;
	  /*	  printf("Timeout %d\n",timeout); */
 	  taskDelay(1);
	  status = *g2ism1;
	  if(status == 0x2 || status_t == 1)   /* Did 8502 issue SRQ in response to command or did we timeout? */
	    status_f = -1;
	  else
	    status_f = 1;
	  break;
	case XMIT_AND_TESTP: 
	  for (i=0; i<strlen(string_array); i++) 
	    {
	      timeout = 0;
	      *g1dido=string_array[i];
		while(*g1ism0 != 0x10 && timeout < MAXWAIT)
		  {
		    timeout++;
		    taskDelay(1);
		  }
		if(timeout >= MAXWAIT)
		  status_t = 1;
		*g2dido=string_array[i]; /* SEND COMMAND STRING TO PPM ONE */
		timeout = 0;
		while(*g2ism0 != 0x10 && timeout < MAXWAIT)
		  {
		    timeout++;
		    taskDelay(1);
		  }
		if(timeout >= MAXWAIT)
		  status_t = 1;
		/*		taskDelay(2);  */  /* CHARACTER AT A TIME */
	    }
	  *g1dido='\n';
	  timeout = 0;
	  while(*g1ism0 != 0x10 && timeout < MAXWAIT)
	    {
	      timeout++;
	      taskDelay(1);
	    }
	  if(timeout >= MAXWAIT)
	    status_t = 1;
	  /*	  taskDelay(1); */
	  *g2dido='\n';
	  timeout = 0;
	  while(*g2ism0 != 0x10 && timeout < MAXWAIT)
	    {
	      timeout++;
	      taskDelay(1);
	    }
	  if(timeout >= MAXWAIT)
	    status_t = 1;
	  status = *g1ism1;
	  status1 = *g2ism1;
	  if(status == 0x2 || status1 == 0x2 || status_t == 1)   /* Did 8502 issue SRQ in response to command or did we timeout? */
	    status_f = -1;
	  else
	    status_f = 1;
	  break;

      }
    if(status_t)
      logMsg("Command Timeout\n",0,0,0,0,0,0);
    return(status_f);
}
