/*  
 *	$Id$
 *
 *	Module:		 ColorSetSvr.c
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

#include "ColorRpc.hh"

#include "stdioLib.h"
#include "string.h"
#include "rpc/pmap_clnt.h"
#include "taskLib.h"

static void colorsetprog_1(struct svc_req *, SVCXPRT *);

int ColorRpcInit(void)
{
    register SVCXPRT *transp;

    (void) pmap_unset(ColorSetProg,ColorSetVers);

    transp = svcudp_create(RPC_ANYSOCK);
    if (transp == NULL) {
	fprintf(stderr,"cannot create udp service.");
	exit(1);
    }

    if (!svc_register(transp,ColorSetProg,ColorSetVers,(int)colorsetprog_1,
		      IPPROTO_UDP))
      {
	  fprintf(stderr,
		  "unable to register (ColorSetProg,ColorSetVers,tcp).");
	return(-1);
      }

    return(0);
}

typedef bool_t (*XDRPTR)();

static void colorsetprog_1(FAST struct svc_req *rqstp, FAST SVCXPRT *transp)
{
    union {
	struct ColorCmd colorsetproc_1_arg;
    } argument;
    char *result;
    XDRPTR xdr_argument;
    XDRPTR xdr_result;
    
    switch (rqstp->rq_proc) {
      case NULLPROC:
	(void) svc_sendreply(transp,xdr_void,(char *)NULL);
	return;
	
      case ColorSetProc:
	xdr_argument = xdr_ColorCmd;
	xdr_result = xdr_void;
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
    result = colorsetproc_1(&argument,rqstp);
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
