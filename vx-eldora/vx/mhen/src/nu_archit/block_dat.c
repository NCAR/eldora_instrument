/*
 *	$Id$
 *
 *	Module: BLOCK_DAT	      
 *	Original Author: E. Loew 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date: 11/20/91		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1994/07/14  20:37:25  eric
 * Initial revision
 *
 * Revision 1.1  1992/11/09  16:07:57  eric
 * Initial revision
 *
 *
 * description: This module provides a block of time series data via a 
 *socket to the SUN4 control processor.
 *              
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define FORE_RADAR
#define NUM_QUAD_DSPS 4
#include <stdioLib.h>
#include <intLib.h>
#include <memLib.h>
#include <semLib.h>
#include <taskLib.h>
#include <vxWorks.h>
#include <ioLib.h>
#include <math.h>
#include "socket.h"
#include "in.h"
#include "systime.h"
#include "ELDRP7.h"
#include "serverDemo.h"
#include "Ray.h"
#include "FieldParam.h"
#include "Platform.h"
#include "coll_dpramdefs.h"
#include "colquadsubs.h"
#include "dspdefs.h"
#include "dspaddr.h"
#include "rp7.h"

IMPORT char sysBootHost [];

#define MAX_CLIENTS 5
#define NUM_FREQS 1
extern int clientSock[MAX_CLIENTS];
extern int curClients;

/* General Initialization */

block_dat(sampl,x)
int   sampl;
float *x;
{
int i,j;

for (i = 0; i < MAX_CLIENTS; ++i) 
  {
      if ( clientSock[i] != ERROR ) 
	{
	    if(send (clientSock[i],x,2*sampl*sizeof(float),0) != 2*sampl*sizeof(float))
	      {
		  printf("\nmsg from fi: Disconnected ");
		  printf("from client %d.\n-> ",curClients);
		  --curClients;
		  close(clientSock[i]);
		  clientSock[i] = ERROR;
		  break;
	      }
	} /* end if client[i] exists */
  }
}












