/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 GeDraw.cc
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.3  1991/10/17  17:16:55  thor
 * Added code to reboot system.
 *
 * Revision 1.2  1991/10/17  16:47:46  thor
 * Increased stack sizes, added test to remove extraneous displays.
 *
 * Revision 1.1  1991/10/14  19:22:02  thor
 * Initial revision
 *
 *
 *
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#include "GeGlobal.hh"
#include "GeDraw.hh"
#include "Ddp.hh"

#include "sysLib.h"
#include "rebootLib.h"

static Task *GetsFlags;

static void DdpLoop(Task &self, Pipe &pipe);

void DrawingLoop(FAST Task &self)


    // Create agc here!
    
    int args[1];
    Rodent = &mouse;


    Task RadialTask((FUNCPTR)RadialLoop,args,1,GRAPH_PRI,6000);
    Task HorizTask((FUNCPTR)HorizLoop,args,1,GRAPH_PRI);
    Task VertTask((FUNCPTR)VertLoop,args,1,GRAPH_PRI);
    Task RadialTask((FUNCPTR)RadialLoop,args,2,GRAPH_PRI,6000);
    Task HorizTask((FUNCPTR)HorizLoop,args,2,GRAPH_PRI);
    Task VertTask((FUNCPTR)VertLoop,args,2,GRAPH_PRI);

    GetsFlags = currTask;

    for (;;)
      {
	  FAST unsigned int flag = self.WaitOnFlags(waitMask | REBOOT |
						    LOAD_ONLY,
						    FLAGS_OR);

		taskDelay(10);
				 // we don't want to start drawing yet.
	    {
		FAST int cmd = (int)GeCommand->cmd;

		switch(cmd)
		  {
		    case FORWARD_RADIAL:
		    case AFT_RADIAL:
		      currTask = &RadialTask;
		      break;

		    case FORWARD_VERT:
		    case AFT_VERT:
		      currTask = &VertTask;
		      break;

		    case FORWARD_HORIZ:
		    case AFT_HORIZ:
		      currTask = &HorizTask;
		  currTask->SetFlags(DESTROY_SELF);
		  {
		      currTask->SetFlags(DESTROY_SELF);
		      taskDelay(30);
		      Task rb((FUNCPTR)Reboot,args,0);
		      return;
		      break;
		  currTask->SetFlags(DESTROY_SELF);
		      
		      currTask->SetFlags(flag);
		      break;
		      
		    case RELOAD:
		    case RESTART:
		  currTask->SetFlags(DESTROY_SELF);
		      if (currTask != &RadialTask)
			    currTask->SetFlags(DESTROY_SELF);
			    taskDelay(30); // We sleep for .5 s to allow
			                   // other active display to
			                   // completely destroy self.
			}
		  currTask->SetFlags(DESTROY_SELF);
		      
		      if (currTask != &RadialTask)
			{
			    currTask->SetFlags(DESTROY_SELF);
			    taskDelay(30);
			}
		  currTask->SetFlags(DESTROY_SELF);
		      
		      if (currTask != &HorizTask)
			{
			    currTask->SetFlags(DESTROY_SELF);
			    taskDelay(30);
			}
		  currTask->SetFlags(DESTROY_SELF);
		      
		      GetsFlags = currTask;
		      currTask->SetFlags(flag);
		      break;
		      
		    case FORWARD_VERT:
		      if (currTask != &VertTask)
			{
			    currTask->SetFlags(DESTROY_SELF);
			    taskDelay(30);
			}
}

static void Reboot(void)
{
    taskDelay(10);
    reboot(BOOT_NORMAL);
}

static void MouseCtrl(Task &self, FAST Mouse &mouse)
{
    for (;;)
      {
         FAST int reason = mouse.waitOnInterrupt();

	 if (reason == MOUSE_INT)
	   GetsFlags->SetFlags(MOUSE_FLAG);
      }
}

static void DdpLoop(Task &self, Pipe &pipe)
{
    Ddp ddp((void *)DDP_ADDR,DDP_VECTOR,pipe);

    DdpCtrl = &ddp;

    for (;;)
      {
	  ddp.Next();
	  GetsFlags->SetFlags(NEW_DATA_FLAG);
      }
}
