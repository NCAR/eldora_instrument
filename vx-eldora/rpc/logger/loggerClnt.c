/*
 *	$Id$
 *
 *	Module:		 loggerClnt.c
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 2.1  1993/09/10  16:42:56  thor
 * New improved version!
 *
 * Revision 2.0  1992/11/02  20:48:31  thor
 * First offical ELDORA release!
 *
 * Revision 1.3  1992/01/16  19:41:47  thor
 * Major rewrite to use semaphore locked statics.
 *
 * Revision 1.2  1992/01/14  19:46:52  thor
 * Added functions to transparently move messages.
 *
 * Revision 1.1  1992/01/14  19:18:16  thor
 * Initial revision
 *
 *
 * description:
 *        This file has several functions used to send log messages.
 * loggerInit takes the name of the UNIX host and sets up the local
 * address & control semaphore. loggerEvent & loggerError take a
 * string and send it to the correct file. They spawn a high priority
 * task to do this. Only one such task may be active at a time.
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define CLIENT_SIDE
#define LOGGER_SCOPE
#include "logger.h"

#include "string.h"
#include "hostLib.h"
#include "rpcLib.h"
#include "socket.h"
#include "taskLib.h"
#include "semLib.h"

static u_long hostnum;		/* Our address. */
static LOG log;			/* Outgoing packet. */
static SEM_ID sem;		/* Control semaphore. */

void logmessage_1(FAST struct LOG *argp)
{
    char res;
    FAST CLIENT *clnt;
    struct timeval tmo = { 10, 0 };
    int s = RPC_ANYSOCK;
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = 0;
    addr.sin_addr.s_addr = hostnum;

    rpcTaskInit();

    clnt = clntudp_create(&addr,Logger,LoggerVers,tmo,&s);

    if (clnt == NULL)
      {
	  clnt_pcreateerror("Client failed: ");
	  semGive(sem);
	  return;
      }

    memset(&res,0,sizeof(res));
        
    if (clnt_call(clnt,LogMessage,xdr_LOG,argp,xdr_void,&res,tmo) != 
	RPC_SUCCESS)
      {
	  LoggerError = ERROR;
	  clnt_perror(clnt,"Failed: ");
	  semGive(sem);
	  return;
      }
    LoggerError = OK;

    clnt_destroy(clnt);
    semGive(sem);
}

void loggerEvent(FAST char *message, FAST int *ip , int num)
{
    semTake(sem,WAIT_FOREVER);

    strncpy(&log.message[0],message,80);

    if (ip != NULL && num > 0)
      memcpy(&log.items[0],ip,num*sizeof(int));

    if (taskSpawn("logEvent",LOGGER_PRI,0,4000,(FUNCPTR)logmessage_1,
		  (int)&log,0,0,0,0,0,0,0,0,0) == ERROR)

      LoggerError = ERROR;
}

int loggerInit(FAST int src)
{
    log.src = src;

    if ((hostnum = (u_long)hostGetByName("odin")) == ERROR)
      return(ERROR);

    if ((sem = semBCreate(SEM_Q_FIFO,SEM_FULL)) == NULL)
      return(ERROR);

    return(OK);
}
