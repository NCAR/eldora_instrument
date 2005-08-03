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
 * Revision 1.5  1995/02/14  16:14:50  thor
 * Fixed things to make it meet ANSI specs.
 *
 * Revision 1.4  1994/09/02  18:20:32  thor
 * Major rewrite.
 *
 * Revision 1.3  1993/08/20  17:43:44  thor
 * *** empty log message ***
 *
 * Revision 1.2  1991/09/06  16:09:39  thor
 * Changed RPC registration routine name, removed rpc initialization
 * & entering of service loop.
 *
 * Revision 1.1  1991/08/28  19:48:18  thor
 * Initial revision
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#include "tapeControl.h"
#include "stdioLib.h"

static void tapecontrol_1(struct svc_req *, SVCXPRT *);

void startControl(void)
{
    FAST SVCXPRT *transp;
    FAST void (*dispatch)() = (void (*)())tapecontrol_1;
    
    (void) pmap_unset(TapeControl,TapeCtrlVers);

    transp = svcudp_create(RPC_ANYSOCK);

    if (transp == NULL)
      {
	  fprintf(stderr,"cannot create udp service.");
	  exit(1);
      }
    
    if (!svc_register(transp,TapeControl,TapeCtrlVers,dispatch,IPPROTO_UDP)) 
      {
	  fprintf(stderr,
		  "unable to register (TapeControl,TapeCtrlVers,udp).");
	  exit(1);
      }
}

static void tapecontrol_1(FAST struct svc_req *rqstp, FAST SVCXPRT *transp)
{
    union {
	struct TapeCommand sendcommand_1_arg;
    } argument;
    FAST char *result;
    FAST bool_t (*xdr_argument)();
    FAST bool_t (*xdr_result)();
    FAST char *(*local)(char *, struct svc_req *);

    switch (rqstp->rq_proc) 
      {
        case NULLPROC:
	  (void) svc_sendreply(transp, xdr_void, (char *)NULL);
	  return;
	  
        case SendCommand:
	  xdr_argument = xdr_TapeCommand;
	  xdr_result = xdr_TapeStatus;
	  local = (char *(*)(char *, struct svc_req *))sendcommand_1_svc;
	  break;
	  
        case GetTapeStatus:
	  xdr_argument = xdr_void;
	  xdr_result = xdr_TapeStatus;
	  local = (char *(*)(char *, struct svc_req *))gettapestatus_1_svc;
	  break;
	  
	  default:
	  svcerr_noproc(transp);
	  return;
      }
    (void)memset((char *)&argument, 0, sizeof(argument));
    if (!svc_getargs(transp, xdr_argument, &argument)) 
      {
	  svcerr_decode(transp);
	  return;
      }

    result = (*local)((char *)&argument, rqstp);

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

#ifdef DEBUG_ONLY

TapeStatus *sendcommand_1_svc(TapeCommand *cmd, struct svc_req *req)
{
    static TapeStatus stat;

    printf("Received command %#x.\n",cmd->cmd);

    stat.drives0[0] = 0;
    stat.drives0[1] = 1;
    stat.drives1[0] = 2;
    stat.drives1[1] = 3;

    stat.number_of_drives[0] = 2;
    stat.number_of_drives[1] = 1;

    stat.status[0] = 0x4;
    stat.status[1] = 0x8;

    stat.failures[0] = 10;
    stat.failures[1] = 1000;

    stat.attempts[0] = 24646;
    stat.attempts[1] = 24646;

    stat.eot_warning = 50;

    stat.unit = 1;

    return(&stat);
}

TapeStatus *gettapestatus_1_svc(void *junk, struct svc_req *req)
{
    static TapeStatus stat;
    static int x = 0;

    printf("Received status request.\n");

    stat.drives0[0] = 0;
    stat.drives0[1] = 1;
    stat.drives1[0] = 2;
    stat.drives1[1] = 3;

    stat.number_of_drives[0] = 2;
    stat.number_of_drives[1] = 1;

    stat.status[0] = 0x4;
    stat.status[1] = 0x8;

    stat.failures[0] = 10;
    stat.failures[1] = 1;

    if (x & 1)
        {

            stat.attempts[0] = x;
            stat.attempts[1] = x;
            stat.unit = 1;
        }
    else
      stat.unit = 0;
    
    stat.eot_warning = 100- (x % 100);

    x++;
    
    return(&stat);
}

#include <rpcLib.h>

void rectst()
{
    rpcTaskInit();

    startControl();

    svc_run();
}

#endif
