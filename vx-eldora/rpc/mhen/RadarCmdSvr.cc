#define OK_RPC
#include "RadarRpc.hh"

#ifdef UNIX
#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#define OK 0
#define ERROR 1
#else
extern "C" {
#include "stdioLib.h"
#include "rpc/pmap_clnt.h"
};
#endif // UNIX

static void radarcontrol_1(struct svc_req *,SVCXPRT *);

int StartRadarCtrl(void)
{
    register SVCXPRT *transp;

    (void)pmap_unset(RadarControl,RadarCtrlVers);
    
    transp = svcudp_create(RPC_ANYSOCK);
    if (transp == NULL)
      {
	  fprintf(stderr,"cannot create udp service.");
	  return(ERROR);
      }
    if (!svc_register(transp,RadarControl,RadarCtrlVers,(void *)radarcontrol_1,
		      IPPROTO_UDP))
      {
	  fprintf(stderr,"unable to register RadarControl,RadarCtrlVers,udp.");
	  return(ERROR);
      }
    return(OK);
}

static void radarcontrol_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
    union
      {
	  struct RadarCommand sendcommand_1_arg;
      } argument;
    char *result;
    xdrproc_t xdr_argument, xdr_result;
    char *(*local)(char *,struct svc_req *);
    
    switch (rqstp->rq_proc)
      {
        case NULLPROC:
	  (void)svc_sendreply(transp,(xdrproc_t)xdr_void,(char *)NULL);
	  return;
	  
        case SendCommand:
	  xdr_argument = (xdrproc_t)xdr_RadarCommand;
	  xdr_result = (xdrproc_t)xdr_RadarStatus;
	  local = (char *(*)(char *,struct svc_req *))sendcommand_1;
	  break;
	  
        case GetRadarStatus:
	  xdr_argument = (xdrproc_t)xdr_void;
	  xdr_result = (xdrproc_t)xdr_RadarStatus;
	  local = (char *(*)(char *,struct svc_req *))getradarstatus_1;
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
	  exit(1);
      }
    return;
}

struct RadarStatus *sendcommand_1(FAST struct RadarCommand *cmdBlk,
				  struct svc_req *)
{
    static u_long count = 0;

    FAST u_long cmd = cmdBlk->cmd;

    if (cmd & INIT)
      count = 0;
    else if (count < cmdBlk->count)
      {
	  printf("Received command 0x%x.\n",cmd);
	  count++;
      }

    currStatus->count++;

    return(currStatus);
}

struct RadarStatus *getradarstatus_1(void *, struct svc_req *)
{
    currStatus->count++;

    return(currStatus);
}
