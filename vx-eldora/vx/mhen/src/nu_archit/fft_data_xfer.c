/*
 *	$Id$
 *
 *	Module:	fft_data_xfer	 
 *	Original Author: Eric Loew
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/11/09  16:07:54  eric
 * Initial revision
 *
 *
 * description: This module opens a socket and provides time series data
 *              to the ELDORA X window FFT TOOL.
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define no_RP7_SCOPE

#include "vxWorks.h"
#include "math.h"
#include "stdioLib.h"
#include "intLib.h"
#include "memLib.h"
#include "semLib.h"
#include "taskLib.h"
#include "tyLib.h"
#include "ioLib.h"
#include "in.h"
#include "systime.h"
#include "sysLib.h"
#include "serverDemo.h"
#include "socket.h"
#include "ELDRP7.h"
#include "coll_dpramdefs.h"
#include "colquadsubs.h"
#include "dspdefs.h"
#include "dspaddr.h"
#include "rp7.h"
#include "RadarGbls.h"

IMPORT char sysBootHost [];
#define MAX_CLIENTS 5

int clientSock[MAX_CLIENTS];
int curClients = 0;

char           *hostName; /* name of boot host */

struct timeval timeOut;
fd_set         readmask;
int            tmp_sock, num_fds;
static int  temp_sampl;
fft_data_xfer()
{

long           i, j;

int            sem_status, server;

float          *x, *data_base;


/* General Initialization */      

temp_sampl = 0;
j = 0;
server = svrStrmSock(SERVER_NUM);
if (server == ERROR)
  {
      free(x);
      printf("fi:couldn't get server socket\n");
      exit(1);
  }
timeOut.tv_sec = 0;
timeOut.tv_usec = 1;
FD_ZERO(&readmask);
for (i = 0; i < MAX_CLIENTS; ++i)
  {
      clientSock[i] = ERROR;
  }

for(;;)
  {
      /* wait here, until someone asks for this service */
      
      printf("\nmsg from fi: Waiting for new connections\n-> ");
      clientSock[0] = svrStrmAccept(server);
      ++curClients;
      printf("\nmsg from fi: Service requested from client...");
      printf("  Connected to client %d\n-> ",curClients);
      while(curClients > 0)
	{
	  /* see if someone else wants to connect */
	    FD_SET(server, &readmask);
	    num_fds = select(FD_SETSIZE, &readmask, NULL, NULL, &timeOut);
	    
	    if (FD_ISSET(server, &readmask) )
	      {                                /* someone wants to talk */
		  tmp_sock = svrStrmAccept(server);
		  if (curClients >=MAX_CLIENTS)
		    {
			printf("\nmsg from fi: Connection refused,");
			printf(" all slots in use\n-> ");
			close(tmp_sock);
		    }
		  else 
		    {
			++curClients;
			for (i = 0; i < MAX_CLIENTS; ++i) 
			  {
			      if (clientSock[i] == ERROR) 
				{
				    clientSock[i] = tmp_sock;
				    printf("\nmsg from fi: Service requested ");
				    printf("from client... Connected to ");
				    printf("client %d\n-> ",curClients);
				    break;
				}
			  }
		    }
	      }

	/* if fft_tool and fft control are ready */

	    if(ts_flag & fft_flag)
	      {

		  /* Define data ray in 680X0 space */

		  if(temp_sampl != ts_samps)
		    {
			if(temp_sampl)
			  free(x);
			temp_sampl = ts_samps;
		  
			if (!(x = (float *)malloc((long)2*temp_sampl*sizeof(float))))
			  printf("680X0 OUT OF MEMORY; ALLOCATION FAILED \n");
		    }

		  data_base =  x;
		  sem_status = semTake(fill_array_sem,WAIT_FOREVER); /* wait until array fill task is available */
		  if(sem_status == OK)
		    {
			a_pntr = x;
			pts = temp_sampl;
			fill_ts_array = 1;
			sem_status = semTake(array_full_sem,WAIT_FOREVER);
			if(sem_status == OK)
			  {
			      semGive(fill_array_sem);
			      block_dat(temp_sampl,data_base);
			      ts_flag = 0;   /* reset time series flag */
			  }
			else
			  printf("Bad array_full semaphore");
		    }
		  else
		    printf("Bad array_fill semaphore! \n");
	      }

	}
      
  }
printf("\n...goodbye\n");
free(x);
return (OK);
}


