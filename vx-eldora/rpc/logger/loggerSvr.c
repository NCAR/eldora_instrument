/*
 *	$Id$
 *
 *	Module:		 loggerSvr.c
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/01/14  19:18:18  thor
 * Initial revision
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#ifndef UNIX
#define UNIX
#endif

#include <stdio.h>
#include "logger.h"

void logger_1(FAST struct svc_req *rqstp, FAST SVCXPRT *transp)
{
    union {
	struct LOG logmessage_1_arg;
    } argument;
    char *result;
    bool_t (*xdr_argument)(), (*xdr_result)();
    char *(*local)();
    
    switch (rqstp->rq_proc)
      {
        case NULLPROC:
	  (void) svc_sendreply(transp,xdr_void,(char *)NULL);
	  return;
	  
        case LogMessage:
	  xdr_argument = xdr_LOG;
	  xdr_result = xdr_void;
	  local = (char *(*)()) logmessage_1;
	  break;
	  
	  default:
	  svcerr_noproc(transp);
	  return;
      }
    bzero((char *)&argument,sizeof(argument));
    if (!svc_getargs(transp,xdr_argument,&argument))
      {
	  svcerr_decode(transp);
	  return;
      }
    result = (*local)(&argument,rqstp);
    if (result != NULL && !svc_sendreply(transp,xdr_result,result))
      {
	  svcerr_systemerr(transp);
      }
    if (!svc_freeargs(transp,xdr_argument,&argument))
      {
	  fprintf(stderr,"unable to free arguments");
	  exit(1);
      }
    return;
}
