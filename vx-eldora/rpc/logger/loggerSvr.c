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
 * Revision 2.1  1994/03/22  16:00:14  thor
 * Changed LOG to LOGMSG, to avoid name conflicts.
 *
 * Revision 2.0  1992/11/02  20:48:31  thor
 * First offical ELDORA release!
 *
 * Revision 2.0  1992/11/02  20:48:31  thor
 * First offical ELDORA release!
 *
 * Revision 1.2  1992/09/14  16:32:30  thor
 * Added stdio.h.
 *
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

extern int _rpcsvcstate;
#define _SERVED 1
#define _SERVING 2

void logger_1(FAST struct svc_req *rqstp, FAST SVCXPRT *transp)
{
    union {
	struct LOGMSG logmessage_1_arg;
    } argument;
    char *result;
    bool_t (*xdr_argument)(), (*xdr_result)();
    char *(*local)(char *, struct svc_req *);

    _rpcsvcstate = _SERVING;
    switch (rqstp->rq_proc)
      {
        case NULLPROC:
	  (void) svc_sendreply(transp,xdr_void,(char *)NULL);
          _rpcsvcstate = _SERVED;
	  return;
	  
        case LogMessage:
	  xdr_argument = xdr_LOGMSG;
	  xdr_result = xdr_void;
	  local = (char *(*)(char *, struct svc_req *)) logmessage_1_svc;
	  break;
	  
	  default:
	  svcerr_noproc(transp);
          _rpcsvcstate = _SERVED;
	  return;
      }
    (void)memset((char *)&argument,0,sizeof(argument));
    if (!svc_getargs(transp,xdr_argument,(caddr_t)&argument))
      {
	  svcerr_decode(transp);
          _rpcsvcstate = _SERVED;
	  return;
      }
    result = (*local)((caddr_t)&argument,rqstp);
    if (result != NULL && !svc_sendreply(transp,xdr_result,result))
      {
	  svcerr_systemerr(transp);
      }
    if (!svc_freeargs(transp,xdr_argument,(caddr_t)&argument))
      {
	  fprintf(stderr,"unable to free arguments");
	  exit(1);
      }
    _rpcsvcstate = _SERVED;
    return;
}
