/*
 *	$Id$
 *
 *	Module:	startup	 
 *	Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/08/19  17:27:19  craig
 * Initial revision
 *
 *
 * description: This module initializes RPC in Housekeeping Processor.
 *              
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#include "Header.hh"
#include "Hskp.hh"

extern "C"{
#include "vxWorks.h"
#include "stdioLib.h"
#include "taskLib.h"
#include "semLib.h"
#include "rpcLib.h"

#include "HskpCmd.h"
#include "HskpStatus.h"

int rpcTaskInit(void);
void startup(void);
};

Header *Hdr;

static HskpStatus status;
static void startRpc(void);

/*********************************************/
/* This is the start of the function startup */
/*********************************************/

void startup(void)
{
    if(taskSpawn("RpcLoop",2,0,6000,(FUNCPTR)startRpc) == ERROR)
      fprintf(stderr,"Failed to start Rpc loop!!!\n");
}

/**********************************************/
/* This is the start of the function startRpc */
/**********************************************/

static void startRpc(void)
{
    currStatus = &status;

    if(rpcTaskInit() == ERROR)
      exit(1);
    startHeader();

/* Start Control Rpc */

    if (HskpRpcInit() != OK)
      {
	  perror("dummy");
	  return;
      }


    svc_run();
    fprintf(stderr,"svc_run returned");  /* this had better never happen! */
}



