#define OK_RPC
#include "RadarRpc.hh"
#include <iostream.h>

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
#include "rpcLib.h"
#include "stdlib.h"
#include "taskLib.h"
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
    FAST void (*dispatch)(...) = (void (*)(...))radarcontrol_1;
    if (!svc_register(transp,RadarControl,RadarCtrlVers,dispatch,IPPROTO_UDP))
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
	  struct RadarCommand sendradarcommand_1_arg;
      } argument;
    char *result;
    xdrproc_t xdr_argument, xdr_result;
    char *(*local)(char *,struct svc_req *);
    
    switch (rqstp->rq_proc)
      {
        case NULLPROC:
	  (void)svc_sendreply(transp,(xdrproc_t)xdr_void,(char *)NULL);
	  return;
	  
        case SendRadarCommand:
	  xdr_argument = (xdrproc_t)xdr_RadarCommand;
	  xdr_result = (xdrproc_t)xdr_RadarStatus;
	  local = (char *(*)(char *,struct svc_req *))sendradarcommand_1_svc;
	  break;
	  
        case GetRadarStatus:
	  xdr_argument = (xdrproc_t)xdr_void;
	  xdr_result = (xdrproc_t)xdr_RadarStatus;
	  local = (char *(*)(char *,struct svc_req *))getradarstatus_1_svc;
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

#ifndef DEBUG_ONLY
struct RadarStatus *sendradarcommand_1_svc(FAST struct RadarCommand *cmdBlk,
                                           struct svc_req *)
{
    FAST u_long cmd = cmdBlk->cmd;

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

    return(currStatus);
}

struct RadarStatus *getradarstatus_1_svc(void *, struct svc_req *)
{
    return(currStatus);
}

#else // !DEBUG_ONLY

struct RadarStatus *getradarstatus_1_svc(void *, struct svc_req *)
{
    static RadarStatus status;

    cout << "Received status request." << endl;
    
    return(&status);
}

struct RadarStatus *sendradarcommand_1_svc(FAST struct RadarCommand *cmdBlk,
                                      struct svc_req *)
{
    static RadarStatus status;

    hex(cout);
    
    cout << "Received command 0x" << cmdBlk->cmd << endl;
    
    return(&status);
}

extern "C" {
    void mhentst();
    void __do_global_ctors(void);
};

void mhentst()
{
    __do_global_ctors();

    rpcTaskInit();

    StartRadarCtrl();

    svc_run();
}

#endif // !DEBUG_ONLY
