#define OK_RPC
#include "RadarRpc.hh"

#ifdef UNIX
#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#define OK 0
#define ERROR 1
#else
#include "stdio.h"
#include "semLib.h"
#include "rpc/pmap_clnt.h"
#include "stdlib.h"
#include "taskLib.h"
#endif // UNIX

static void radarcontrol_1(struct svc_req *,SVCXPRT *);

int StartRadarCtrl(void)
{
    register SVCXPRT *transp;

    (void)pmap_unset(RadarControl,RadarCtrlVers);
    
    transp = svcudp_create(RPC_ANYSOCK,8192,8192);
    if (transp == NULL)
      {
	  fprintf(stderr,"cannot create udp service.");
	  return(ERROR);
      }
    if (!svc_register(transp,RadarControl,RadarCtrlVers,
		      radarcontrol_1,IPPROTO_UDP))
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
    if(cmd & LOAD)
      load = 1;

    if(cmd & START)
      {
/*	  cmd &= !STOP; */      /* clear out stop flag in case STOP was hit twice */
	  semGive(exec_sem);
	  semGive(real_sem);
      }
    if(cmd & STOP)
      stop = 1;
    if(cmd & REBOOT)
      reboot = 1;
    if(cmd & RELOAD)
      {
	  stop = 1;
	  taskDelay(10); /* add short delay to allow coll_data_xfer to finish */
	  semGive(exec_sem);
	  semGive(real_sem);
      }
    if(cmd & DC_REMOVAL)
      {    
	  dc_removal = 1;
	  currStatus->count++;
	  currStatus->rp7 |= DC_BUSY;
	  printf("current rp7 status = %X \n",currStatus->rp7);
      }
    if(cmd & DC_1)
	  ad_freq = 1;
    if(cmd & DC_2)
	  ad_freq = 2;
    if(cmd & DC_3)
	  ad_freq = 3;
    if(cmd & AD_CHAN_LO)
      ad_chan = (char )1;
    if(cmd & AD_CHAN_HI)
      ad_chan = (char )0;
    if(cmd & RESYNC)
      {    
	  resync = 1;
      }
    if(cmd & FFT)
      fft_flag = 1;
    if(cmd & FFT_1)
      ts_freq = 1;
    if(cmd & FFT_2)
      ts_freq = 2;
    if(cmd & FFT_3)
      ts_freq = 3;

#ifdef TASS
    if (cmd & TS_ON)
      {
	  memcpy((char *)tass_ptr,(char *)&(cmdBlk->tass_info),sizeof(TSACQ));
	  /* Now whatever turns on time series.*/
	  ts_acq_on = 1;
      }
    if (cmd & TS_OFF)
      ts_acq_on = 0; /* Whatever turns it off. */

#endif /* TASS */
	
    printf("ts_freq = %d \n",ts_freq);
    currStatus->count++;

    return(currStatus);
}

struct RadarStatus *getradarstatus_1(void *, struct svc_req *)
{
    currStatus->count++;

    return(currStatus);
}














