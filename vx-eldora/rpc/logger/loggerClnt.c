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
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define CLIENT_SIDE
#include "logger.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

void *logmessage_1(FAST struct LOG *argp, FAST CLIENT *clnt)
{
        static char res;

        bzero((char *)&res,sizeof(res));
        if (clnt_call(clnt,LogMessage,xdr_LOG,argp,xdr_void,&res,TIMEOUT) != 
	    RPC_SUCCESS)
	  {
	      return(NULL);
	  }
        return((void *)&res);
}
