//  This looks like C, but it's really C++!!
//	$Id$
//
//	Module:		 HeaderRpc_svc.cc
//	Original Author: Richard E. Neitzel
//      Copywrited by the National Center for Atmospheric Research
//	Date:		 $Date$
//
// revision history
// ----------------
// $Log$
//
// description:
//        
//
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "HeaderRpc.h"
#include <string.h>
#include <stdlib.h>

static void headerrpc_1(FAST struct svc_req *rqstp, FAST SVCXPRT *transp)
{
    union {
        struct headername headerrpcfunc_1_arg;
    } argument;
    FAST char *result;
    FAST xdrproc_t xdr_argument, xdr_result;
    FAST char *(*local)(char *, struct svc_req *);

    switch (rqstp->rq_proc)
      {
        case NULLPROC:
            (void) svc_sendreply(transp,
                                 (xdrproc_t) xdr_void,(char *)NULL);
            return;

        case HeaderRpcFunc:
            xdr_argument = (xdrproc_t) xdr_headername;
            xdr_result = (xdrproc_t) xdr_int;
            local = (char *(*)(char *, struct svc_req *)) headerrpcfunc_1_svc;
            break;

        default:
            svcerr_noproc(transp);
            return;
      }
    (void) memset((char *)&argument,0,sizeof (argument));
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

void HeaderRpcInit()
{
    FAST SVCXPRT *transp = svcudp_create(RPC_ANYSOCK);

    if (transp == NULL)
      {
          cerr << "Cannot create udp service." << endl;
          return;
      }

    FAST void (*dispatch)(...) = (void (*)(...))headerrpc_1;
    
    if (!svc_register(transp,HeaderRpc,HeaderRpcVers,dispatch,IPPROTO_UDP))
      {
          cerr << "Unable to register (HeaderRpc, HeaderRpcVers)." << endl;
          return;
      }

    transp = svctcp_create(RPC_ANYSOCK,0,0);

    if (transp == NULL)
      {
          cerr << "Cannot create tcp service." << endl;
          return;
      }
  
    if (!svc_register(transp,HeaderRpc,HeaderRpcVers,dispatch,IPPROTO_TCP))
      {
          cerr << "Unable to register (HeaderRpc, HeaderRpcVers)." << endl;
          return;
      }
    
    svc_run();
}
