/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 HpaRpcSvr.cc
 *	Original Author: Richard E. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#include "HpaRpc.hh"
#include <stream.h>
#include <rpc/pmap_clnt.h>

static void hparpcprog_1(struct svc_req *,SVCXPRT *);

int HpaRpcInit(int hpa)
{
    register SVCXPRT *transp;
    
    if (hpa == 1)
      (void)pmap_unset(HpaRpcProg1,HpaRpcVers);
    else
      (void)pmap_unset(HpaRpcProg2,HpaRpcVers);

    transp = svcudp_create(RPC_ANYSOCK);
    if (transp == NULL)
      {
	  cerr << "cannot create udp service." << endl;
	  return(-1);
      }

    if (hpa == 1)
      {
	  if (!svc_register(transp,HpaRpcProg1,HpaRpcVers,hparpcprog_1,
			    IPPROTO_UDP))
	    {
		cerr << "unable to register (HpaRpcProg,HpaRpcVers,udp)." << 
		  endl;
		return(-1);
	    }
      }
    else
      {
	  if (!svc_register(transp,HpaRpcProg2,HpaRpcVers,hparpcprog_1,
			    IPPROTO_UDP))
	    {
		cerr << "unable to register (HpaRpcProg,HpaRpcVers,udp)." << 
		  endl;
		return(-1);
	    }
      }
    return(0);
}

static void hparpcprog_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
    union {
	enum HPA_CMD hpasendcmd_1_arg;
    } argument;
    char *result;
    xdrproc_t xdr_argument,xdr_result;
    char *(*local)(char *,struct svc_req *);
    
    switch(rqstp->rq_proc)
      {
        case NULLPROC:
	  (void) svc_sendreply(transp,(xdrproc_t) xdr_void,(char *)NULL)
	    ;
	  return;
	  
        case HpaSendCmd:
	  xdr_argument = (xdrproc_t) xdr_HPA_CMD;
	  xdr_result = (xdrproc_t) xdr_void;
	  local = (char *(*)(char *,struct svc_req *)) hpasendcmd_1;
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
	  cerr << "unable to free arguments" << endl;
	  exit(1);
      }
    return;
}
