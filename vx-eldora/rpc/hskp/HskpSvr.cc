/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 HskpSvr.cc
 *	Original Author: Richard E. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/07/13  20:18:57  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define HSKP_RPC_SCOPE
#include "Hskp.hh"

extern "C" {
int HskpRpcInit(void);
#include "stdioLib.h"
#include "rpc/pmap_clnt.h"
};

static void hskpcontrol_1(struct svc_req *,SVCXPRT *);
static HskpStatus status;

int HskpRpcInit(void)
{
    FAST SVCXPRT *transp;

    currStatus = &status;
    
    (void)pmap_unset(HskpControl,HskpCtrlVers);
    
    transp = svcudp_create(RPC_ANYSOCK);
    if (transp == NULL)
      {
	  fprintf(stderr,"cannot create udp service.");
	  return(ERROR);
      }
    if (!svc_register(transp,HskpControl,HskpCtrlVers,hskpcontrol_1,
		      IPPROTO_UDP))
      {
	  fprintf(stderr,"unable to register (HskpControl,HskpCtrlVers,udp).");
	  return(ERROR);
      }
    
    return(OK);
}

static void hskpcontrol_1(FAST struct svc_req *rqstp, FAST SVCXPRT *transp)
{
    union {
	struct HskpCommand sendcommand_1_arg;
    } argument;
    char *result;
    xdrproc_t xdr_argument,xdr_result;
    char *(*local)(char *,struct svc_req *);
    
    switch (rqstp->rq_proc) 
      {
	case NULLPROC:
	  (void)svc_sendreply(transp,(xdrproc_t) xdr_void,(char *)NULL);
	  return;
	  
	case SendCommand:
	  xdr_argument = (xdrproc_t)xdr_HskpCommand;
	  xdr_result = (xdrproc_t)xdr_HskpStatus;
	  local = (char *(*)(char *,struct svc_req *))sendcommand_1;
	  break;
	  
	case GetHskpStatus:
	  xdr_argument = (xdrproc_t)xdr_void;
	  xdr_result = (xdrproc_t)xdr_HskpStatus;
	  local = (char *(*)(char *,struct svc_req *))gethskpstatus_1;
	  break;
	  
	default:
	  svcerr_noproc(transp);
	  return;
      }
    bzero((char *)&argument,sizeof(argument));
    if (!svc_getargs(transp,xdr_argument,(caddr_t) &argument))
      {
	  svcerr_decode(transp);
	  return;
      }
    result = (*local)((char *)&argument,rqstp);
    if (result != NULL && !svc_sendreply(transp,xdr_result,result))
      {
	  svcerr_systemerr(transp);
      }
    if (!svc_freeargs(transp,xdr_argument,(caddr_t) &argument))
      {
	  fprintf(stderr,"unable to free arguments");
	  return;
      }
    return;
}

struct HskpStatus *sendcommand_1(FAST struct HskpCommand *cmdBlk,
				 struct svc_req *)
{
    static u_long count = 0;

    FAST u_long cmd = cmdBlk->cmd;

    if (cmd & INIT)
      count = 0;
    else if (count < cmdBlk->count)
      {
	      // This is where your code goes!
      }

    currStatus->count++;

    return(currStatus);
}

struct HskpStatus *gethskpstatus_1(void *, struct svc_req *)
{
    currStatus->count++;

    return(currStatus);
}
