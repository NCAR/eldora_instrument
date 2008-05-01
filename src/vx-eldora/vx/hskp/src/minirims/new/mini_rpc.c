/*
 *      $Id$
 *
 *      Module:          mini_rpc.c
 *      Original Author: Rich Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *      Date:            $Date$
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
#define NO_MIN_SCOPE
#include "MinirimsRpc.hh"
#include <taskLib.h>
#include "GblCommands.h"
#include "minGbl.h"
#include "numinDef.h"
#include "stdioLib.h"
#include "semLib.h"

static void rpcloop();

void startrpc()
{
  if(taskSpawn("rpcLoop",70,0,5000,(FUNCPTR)rpcloop,0,0,0,0,0,0,0,0,0,0)==ERROR)
  fprintf(stderr,"Failed to start Rpc loop!!!\n");

/* Create required semaphores */
  
  main_sem = semBCreate(SEM_Q_FIFO,SEM_EMPTY);

  /* Initialize global pointers */

Command=(unsigned short *)(MIN_BASE+MIN_COMMAND);

   /* Allocate space for global structures */

    if (!(ovp = (MINI_OV1 *)malloc(sizeof(MINI_OV1))))
      fprintf(stderr,"680X0 OUT OF MEMORY; ALLOCATION FAILED \n");
    if (!(dp = (MINI_OV1 *)malloc(sizeof(MINI_OV1))))
      fprintf(stderr,"680X0 OUT OF MEMORY; ALLOCATION FAILED \n");
}

static void rpcloop()
{
  rpcTaskInit();

  MinirimsRpcInit();

  svc_run();
}

int *sendminirimscommand_1_svc(struct MinirimsCommand *cmdBlk,
			       struct svc_req *x)
{
  static int return_value;

  /* Your stuff here */

  if(cmdBlk->cmd & START)  /* Start main task */
    {
      semGive(main_sem);   
    }
  if(cmdBlk->cmd & STOP)   /* Stop all tasks */
    {
      Stop = 1;
    }
  return(&return_value);
}
