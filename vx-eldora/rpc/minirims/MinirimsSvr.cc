//  This looks like C, but it's really C++!!
//      $Id$
//
//      Module:          MinirimsSvr.cc
//      Original Author: Rich Neitzel
//      Copywrited by the National Center for Atmospheric Research
//      Date:            $Date$
//
// revision history
// ----------------
// $Log$
//
//
// description:
//        
//
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC

#include "MinirimsRpc.hh"

static void minirimscontrol_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
  union {
    struct MinirimsCommand sendminirimscommand_1_arg;
  } argument;
  char *result;
  xdrproc_t _xdr_argument, _xdr_result;
  char *(*local)(char *, struct svc_req *);

  switch (rqstp->rq_proc)
    {
    case NULLPROC:
      (void) svc_sendreply(transp,(xdrproc_t) xdr_void, (char *)NULL);
      return;

    case sendMinirimsCommand:
      _xdr_argument = (xdrproc_t) xdr_MinirimsCommand;
      _xdr_result = (xdrproc_t) xdr_int;
      local = (char *(*)(char *, struct svc_req *)) sendminirimscommand_1_svc;
      break;

    default:
      svcerr_noproc(transp);
      return;
    }
  (void) memset((char *)&argument, 0, sizeof (argument));
  if (!svc_getargs(transp, _xdr_argument, (caddr_t) &argument))
    {
      svcerr_decode(transp);
      return;
    }
  result = (*local)((char *)&argument, rqstp);
  if (result != NULL && !svc_sendreply(transp, _xdr_result, result))
    {
      svcerr_systemerr(transp);
    }
  if (!svc_freeargs(transp, _xdr_argument, (caddr_t) &argument))
    {
      cerr << "MinirimsCtrl unable to free arguments" << endl;
      exit(1);
    }

  return;
}

void MinirimsRpcInit()
{
  FAST SVCXPRT *transp = svcudp_create(RPC_ANYSOCK);

  if (transp == NULL)
    {
      cerr << "Cannot create udp service." << endl;
      return;
    }

  FAST void (*dispatch)(...) = (void (*)(...))minirimscontrol_1;
    
  if (!svc_register(transp,MinirimsControl,MinirimsCtrlVers,dispatch,
		    IPPROTO_UDP))
    {
      cerr << "Unable to register DispControl." << endl;
      return;
    }
}
