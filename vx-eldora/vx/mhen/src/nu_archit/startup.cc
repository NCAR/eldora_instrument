/*
 *	$Id$
 *
 *	Module:	startup	 
 *	Original Author: Eric Loew 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1994/07/14  20:32:52  eric
 * Initial revision
 *
 * Revision 1.1  1992/11/09  16:07:24  eric
 * Initial revision
 *
 * Revision 1.1  1991/12/09  22:12:32  eric
 * Initial revision
 *
 *
 * description: This module initializes RPC in Radar Processor.
 *              
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#include "Header.hh"
#include "RadarRpc.hh"
#include "HeaderRpc.h"

extern "C"{
#include "vxWorks.h"
#include "stdioLib.h"
#include "taskLib.h"
#include "semLib.h"
#include "rpcLib.h"
#define RDP_SCOPE
#include "RDPGlobals.h"
#include "RadarGbls.h"
int rpcTaskInit(void);
void startup(void);
int init_fft_cmds_serv();
};
Header *Hdr;
char *PN;
static RadarStatus status;
static void startRpc(void);
void startup(void)
{
    if(taskSpawn("RpcLoop",2,0,12000,(FUNCPTR)startRpc,0,0,0,0,0,0,0,0,0,0) == ERROR)
      fprintf(stderr,"Failed to start Rpc loop!!!\n");
}
static void startRpc(void)
{
    currStatus = &status;

/* Create required semaphores */
  
    exec_sem = semBCreate(SEM_Q_FIFO,SEM_EMPTY);
    real_sem = semBCreate(SEM_Q_FIFO,SEM_EMPTY);
    fill_array_sem = semBCreate(SEM_Q_FIFO,SEM_FULL);
    array_full_sem = semBCreate(SEM_Q_FIFO,SEM_EMPTY);

/* Create the required semaphores for each BIM ISR */

exec_int0_sem = semBCreate(SEM_Q_FIFO,SEM_EMPTY);
bim_int0_sem = semBCreate(SEM_Q_FIFO,SEM_EMPTY);
bim_int1_sem = semBCreate(SEM_Q_FIFO,SEM_EMPTY);
bim_int2_sem = semBCreate(SEM_Q_FIFO,SEM_EMPTY);
bim_int3_sem = semBCreate(SEM_Q_FIFO,SEM_EMPTY);

    if(rpcTaskInit() == ERROR)
      exit(1);
    startHeader();

/* Start Control Rpc */

    if (StartRadarCtrl() != OK)
      {
	  perror("dummy");
	  return;
      }

/* DEFINE PROCESS NAME */
    PN = "fftserver";

/* Register the RPC services */

    if(init_fft_cmds_serv() != 0)
      {
	  fprintf(stderr, "%s: Bad fft rpc server init", PN);
	  fprintf(stderr,"bye !\n");
	  exit(1);
      }
    svc_run();
    fprintf(stderr,"svc_run returned");  /* this had better never happen! */
}



