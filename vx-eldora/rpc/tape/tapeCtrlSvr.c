/*
 *	$Id$
 *
 *	Module:		 tapeCtrlSvr.c
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
#include "tapeControl.h"
#include "stdioLib.h"

static void tapecontrol_1(struct svc_req *, SVCXPRT *);

void startRpc(void)
{
    FAST SVCXPRT *transp;

    if (rpcTaskInit() == ERROR)
      exit(1);

    (void) pmap_unset(TapeControl,TapeCtrlVers);

    transp = svcudp_create(RPC_ANYSOCK);

    if (transp == NULL)
      {
	  fprintf(stderr,"cannot create udp service.");
	  exit(1);
      }

    if (!svc_register(transp,TapeControl,TapeCtrlVers,(int)tapecontrol_1,
		      IPPROTO_UDP)) 
      {
	  fprintf(stderr,
		  "unable to register (TapeControl,TapeCtrlVers,udp).");
	  exit(1);
      }

    svc_run();
    fprintf(stderr, "svc_run returned");
    exit(1);
    /* NOTREACHED */
}

static void tapecontrol_1(FAST struct svc_req *rqstp, FAST SVCXPRT *transp)
{
    union {
	struct TapeCommand sendcommand_1_arg;
    } argument;
    FAST char *result;
    FAST bool_t (*xdr_argument)();
    FAST bool_t (*xdr_result)();
    FAST char *(*local)();

    switch (rqstp->rq_proc) 
      {
        case NULLPROC:
	  (void) svc_sendreply(transp, xdr_void, (char *)NULL);
	  return;
	  
        case SendCommand:
	  xdr_argument = xdr_TapeCommand;
	  xdr_result = xdr_TapeStatus;
	  local = (char *(*)()) sendcommand_1;
	  break;
	  
        case GetTapeStatus:
	  xdr_argument = xdr_void;
	  xdr_result = xdr_TapeStatus;
	  local = (char *(*)()) gettapestatus_1;
	  break;
	  
	  default:
	  svcerr_noproc(transp);
	  return;
      }
    bzero((char *)&argument, sizeof(argument));
    if (!svc_getargs(transp, xdr_argument, &argument)) 
      {
	  svcerr_decode(transp);
	  return;
      }
    result = (*local)(&argument, rqstp);

    if (result != NULL && !svc_sendreply(transp, xdr_result, result)) 
      {
	  svcerr_systemerr(transp);
      }
    if (!svc_freeargs(transp, xdr_argument, &argument)) 
      {
	  fprintf(stderr, "unable to free arguments");
	  exit(1);
      }
    return;
}
