/*
 *	$Id$
 *
 *	Module:		 Logger.c
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description:
 *        This program opens a single event log file & seperate error
 * logs for each target. 
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


#define UNIX
#define OK_RPC
#include "logger.h"

static FILE *eventFp;		/* Event log file. */
static char *directory = ".";	/* Default directory path. */
static char *hosts[] =		/* Know targets. */
{ "frey", "tyr", "ull", "njord", "jord", "sif", "baldr", "hel", NULL };
static FILE *errorFps[8];	/* Their error logs. */

main(FAST int argc, FAST char **argv)
{
    FAST SVCXPRT *transp;
    FAST FILE **fps;
    FAST int i;
    FAST int j = 8;
    char buffer[256];

    if (argc > 1)
      directory = argv[1];

    /* Null the pointers so we can determine who is/isn't open. */
    for (i = 0, fps = errorFps; i < j; i++, fps++)
      *fps = NULL;

    strcpy(buffer,directory);
    strcat(buffer,"/event.log");

    eventFp = fopen(buffer,"a+");

    (void) pmap_unset(Logger, LoggerVers);
    
    transp = svcudp_create(RPC_ANYSOCK);

    if (transp == NULL)
      {
	  fprintf(stderr,"%s: cannot create udp service.",*argv);
	  exit(1);
      }

    if (!svc_register(transp,Logger,LoggerVers,logger_1,IPPROTO_UDP))
      {
	  fprintf(stderr," %s: unable to register(Logger, LoggerVers, udp).",
		  *argv);
	  exit(1);
      }
    
    svc_run();

    fprintf(stderr, "svc_run returned");
    exit(1);
}

void *logmessage_1(FAST LOG *argp, FAST struct svc_req *req)
{
    FAST int len = strlen(&argp->message[0]);

    if (argp->file == EVENTLOG)	/* Event message. */
      {
	  fwrite(&argp->message[0],sizeof(char),len,eventFp);
	  fflush(eventFp);	/* Force writes. */
      }
    else
      {
	  /* the following extracts the network address from the */
	  /* request and determines the host name from it. */
	  FAST SVCXPRT *svcptr = req->rq_xprt;
	  FAST struct sockaddr_in *addrPtr = svc_getcaller(svcptr);
	  FAST struct hostent *host = gethostbyaddr(&addrPtr->sin_addr.s_addr,
						    4,AF_INET);
	  FAST char *name = *(host->h_aliases);
	  
	  FAST char **ptr = hosts;

	  FAST int count = 0;

	  while (*ptr != NULL)
	    {
		if (!strncmp(*ptr,name,strlen(*ptr))) /* A hit. */
		  {
		      if (errorFps[count] == NULL) /* Not open yet. */
			{
			    char buffer[256];

			    strcpy(buffer,directory);
			    strcat(buffer,"/");
			    strcat(buffer,name);
			    strcat(buffer,".log");

			    errorFps[count] = fopen(buffer,"a+");
			}
		      fwrite(&argp->message[0],sizeof(char),len,
			     errorFps[count]);
		      fflush(errorFps[count]);

		      return(argp);
		  }
		ptr++;
	    }
      }
    return(argp);
}
