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
 * Revision 1.1  1992/01/14  19:18:16  thor
 * Initial revision
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define CLIENT_SIDE
#define LOGGER_SCOPE
#include "logger.h"

#include "strLib.h"
#include "taskLib.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

static CLIENT *Client;

void logmessage_1(FAST struct LOG *argp, FAST CLIENT *clnt)
{
        char res;

        bzero((char *)&res,sizeof(res));
        
	if (clnt_call(clnt,LogMessage,xdr_LOG,argp,xdr_void,&res,TIMEOUT) != 
	    RPC_SUCCESS)
	  {
	      LoggerError = ERROR;
	  }
        LoggerError = OK;
}

void loggerError(FAST char *message)
{
    LOG log;

    strncpy(&log.message[0],message,255);

    log.message[255] = 0;

    log.file = ERRORLOG;

    if (taskSpawn("loggerError",LOGGER_PRI,0,4000,(FUNCPTR)logmessage_1,
		  (int)&log,(int)Client) == ERROR)
      LoggerError = ERROR;
}

void loggerEvent(FAST char *message)
{
    LOG log;

    strncpy(&log.message[0],message,255);

    log.message[255] = 0;

    log.file = EVENTLOG;

    if (taskSpawn("loggerError",LOGGER_PRI,0,4000,(FUNCPTR)logmessage_1,
		  (int)&log,(int)Client) == ERROR)
      LoggerError = ERROR;
}

int loggerInit(FAST char *host)
{
    Client = clnt_create(host,Logger,LoggerVers,"udp");

    if (Client == NULL)
      return(ERROR);

    return(OK);
}
