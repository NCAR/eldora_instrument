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
 * Revision 1.2  1992/07/27  14:46:05  thor
 * Added extern "C" for HskpRpcInit.
 *
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

#include <stdioLib.h>
#include <sysLib.h>
#include <rebootLib.h>
#include <rpc/pmap_clnt.h>

#ifdef DEBUG_ONLY
#include <rpcLib.h>

extern "C" {
    void __do_global_ctors(void);
    void hskptst(void);
};
#endif


/* include fifty million .h files to deal with the header formats */

#include "Volume.h"
#include "Waveform.h"
#include "RadarDesc.h"
#include "FieldRadar.h"
#include "CellSpacing.h"
#include "Parameter.h"
#include "NavDesc.h"
#include "InSitu.h"
#include "Ray.h"
#include "Platform.h"
#include "FieldParam.h"
#include "IndFreq.h"
#include "TimeSeries.h"
#include "NavInfo.h"
#include "Ins.h"
#include "MiniRIMS.h"
#include "Gps.h"
#include "InSituData.h"

/* include the .h files that are housekeeper code specific */
extern "C" {
#define scope extern
#include "vme_hndshk.h"
#include "vmevmeGbl.h"
#include "hskpDef.h"
#include "hskpGbl.h"
#include "todGbl.h"
#include "todFunc.h"
#include "gpsFunc.h"
#include "minFunc.h"
#include "cntrlFunc.h"
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

    FAST void (*dispatch)(...) = (void (*)(...))hskpcontrol_1;
    
    if (!svc_register(transp,HskpControl,HskpCtrlVers,dispatch,IPPROTO_UDP))
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
	  local = (char *(*)(char *,struct svc_req *))sendcommand_1_svc;
	  break;
	  
	case GetHskpStatus:
	  xdr_argument = (xdrproc_t)xdr_void;
	  xdr_result = (xdrproc_t)xdr_HskpStatus;
	  local = (char *(*)(char *,struct svc_req *))gethskpstatus_1_svc;
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

#ifndef DEBUG_ONLY
struct HskpStatus *sendcommand_1_svc(FAST struct HskpCommand *cmdBlk,
                                     struct svc_req *)
{
    float position;
    char year,date,hour,minute,second,month,test;
    long i;

    FAST u_long cmd = cmdBlk->cmd;

    printf("command received: %lx\n",cmd);
    if (cmd & INIT)
      ;
    else
      {
	      /***********************************************/
	      /**** Start of code inserted by Craig Walther **/
	      /***********************************************/
	  if(cmd & START_CLOCK)  /* Has Control processor told us to
                                    start clock? */
	    {
		start_clock();
		currStatus->clock = 0;
	    }
	  if(cmd & START)  /* Has control Processor told us to start? */
	    {
		stop_flag = 0;
	    }
	  if(cmd & STOP)   /* Has control Processor told us to stop? */
	    {
		stop_flag = 1;
	    }
	  if(cmd & RELOAD)   /* Has control Processor told us to reload? */
	    {
		reload_flag = 1;
	    }
	  if(cmd & REBOOT)   /* Has control Processor told us to reboot? */
	    reboot((int)BOOT_NORMAL);
	  if(cmd & ALIGN)    /* Has control Processor told us to align
	                        the miniRIMS IRU? */
	    {
		/*	position = ENCODER_ZERO_FORE_UP; */
		goto_pos(position);
		align_mini();
	    }
	  if(cmd & GPS_START)  /* Has the control processor told us to
	                          program the TANS II GPS receiver? */
	    {
		command_gps((short)2);
	    }
	  if(cmd & IRIGB_SYNC) /* Has the control processor told us to sync
                                  the time of day card to the ADS's
                                  IRIG-B signal? */
            {
               currStatus->clock = IRIGB_SYNCING;
               test = sync_irig();
               if(test)            /* Were we successful? */
                   currStatus->clock = 0;
               else
                   currStatus->clock = IRIGB_SYNC_FAILED;
             }
           if(cmd & SET_TIME)  /* Has the Control Processor told us to set
                                  the time? */
             {
               year = cmdBlk->year;
               month = cmdBlk->month;
               date = cmdBlk->day;
               hour = cmdBlk->hour;
               minute = cmdBlk->minute;
               second = cmdBlk->second;
               set_time(hour,minute,second,month,date,year);

               /* Check for leap year, update the jday_calc array if it is */
               if((((int)(year/4))*4 == year) && (jday_calc[2] == 60))
                 {
                 for(i=2; i<12; i++)
	           jday_calc[i]++;
                 }
               currStatus->clock = TIME_SET_READY;
             }
      }
    return(currStatus);
}

struct HskpStatus *gethskpstatus_1_svc(void *, struct svc_req *)
{
    return(currStatus);
}

#else // DEBUF_ONLY

static HskpStatus stat;

struct HskpStatus *sendcommand_1_svc(FAST struct HskpCommand *cmdBlk,
                                     struct svc_req *)
{
    printf("Received command %#x.\n",cmdBlk->cmd);

    memset(&stat,0,sizeof(stat));

    return(&stat);
}

struct HskpStatus *gethskpstatus_1_svc(void *, struct svc_req *)
{
    // Send back phony error indications.

    printf("Received status request.\n");
    stat.fore = 1;
    stat.onBoverT = 2;
    stat.gps = 3;
    return(&stat);
}

void hskptst()
{
    __do_global_ctors();

    rpcTaskInit();

    HskpRpcInit();

    svc_run();
}

#endif // DEBUG_ONLY
