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
#include "minDef.h"
#include "stdioLib.h"
#include "semLib.h"

#include <cstring>
#include "udpSvr.hh"
#include "Task.hh"

void cmdloop();

extern "C" {
  void startrpc();
  void __do_global_ctors(void);
};

void startrpc()
{
  if(taskSpawn("cmdLoop",70,0,5000,(FUNCPTR)cmdloop,0,0,0,0,0,0,0,0,0,0)==ERROR)
  fprintf(stderr,"Failed to start Cmd loop!!!\n");

/* Create required semaphores */
  
  main_sem = semBCreate(SEM_Q_FIFO,SEM_EMPTY);

  /* Initialize global pointers */

Command=(unsigned short *)(MIN_BASE+MIN_COMMAND);

   /* Allocate space for global structures */

    if (!(ovp = (MINI_OV1 *)malloc(sizeof(MINI_OV1))))
      fprintf(stderr,"680X0 OUT OF MEMORY; ALLOCATION FAILED \n");
    if (!(dp = (MINI_D *)malloc(sizeof(MINI_D))))
      fprintf(stderr,"680X0 OUT OF MEMORY; ALLOCATION FAILED \n");
}

void cmdloop()
{
  // Initialize all globally declared objects.
  __do_global_ctors();
  cout.sync_with_stdio(0);

  udpSvr svr(3000);

  MinirimsCommand cmd;

  while (1)
    {
      int bytes = svr.recvfrom((void *)&cmd,sizeof(cmd));

      if ( bytes < sizeof(cmd))
	{
	  if (bytes < 0)
	    {
	      cerr << "udpSvr: " << strerror(errno) << endl;
	      continue;
	    }
	  else
	    {
	      cerr << "udpSvr only got " << bytes << " bytes.\n";
	      continue;
	    }
	}
      /* Your stuff here */

      if(cmd.cmd & START)  /* Start main task */
	{
	  cout << "Got start command." << endl;
	  semGive(main_sem);   
	}

      if(cmd.cmd & STOP)   /* Stop all tasks */
	{
	  cout << "Got stop command." << endl;
	  Stop = 1;
	}

      svr.setAddr((struct sockaddr_in *)svr.getAddr());

      int return_value = 0;

      svr.sendto((void *)&return_value,sizeof(int));
    }
}
