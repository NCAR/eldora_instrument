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
 * Revision 2.0  1992/11/02  20:48:31  thor
 * First offical ELDORA release!
 *
 * Revision 1.2  1992/09/14  16:31:52  thor
 * Added new host & made things a little clearer.
 *
 * Revision 1.1  1992/01/16  19:40:11  thor
 * Initial revision
 *
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

#ifndef KNOWN_HOSTS
#define KNOWN_HOSTS 6
#endif

static FILE *eventFps[KNOWN_HOSTS]; /* Event log file. */
static char *directory = ".";	/* Default directory path. */
static char *files[] = {
"fore.log", "aft.log", "disp1.log", "disp2.log", "record.log", "hskp.log"
};

main(FAST int argc, FAST char **argv)
{
    FAST SVCXPRT *transp;
    FAST FILE **fps;
    FAST int i;
    FAST int j = KNOWN_HOSTS;

    if (argc > 1)
      directory = argv[1];

    /* Open the files for append access. */
    for (i = 0, fps = eventFps; i < j; i++, fps++)
      {
	  char buffer[256];

	  strcpy(buffer,directory);
	  strcat(buffer,"/");
	  strcat(buffer,files[i]);

      if ((*fps = fopen(buffer,"a+")) == NULL)
	{
	    perror(argv[0]);
	    exit(1);
	}
      }

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

    FAST FILE *fp = eventFps[argp->src];

    fprintf(fp,argp->message,argp->items[0],argp->items[1],
	    argp->items[2],argp->items[3]);

    fflush(fp);			/* Force output to be written now. */

    return(argp);
}
