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
 * Revision 2.0  1992/11/03  12:53:30  thor
 * First offical ELDORA release!
 *
 * Revision 1.15  1992/10/21  14:33:40  thor
 * Cleaned up wait flags & corrected LOAD_ONLY test.
 *
 * Revision 1.14  1992/10/09  14:47:33  thor
 * Added Setting of LOAD_ONLY task for current task.
 *
 * Revision 1.13  1992/10/02  20:45:23  thor
 * Added LOAD_ONLY support.
 *
 * Revision 1.12  1992/06/29  17:31:19  thor
 * Added global Ddp pointer. Increased pipe size.
 *
 * Revision 1.11  1992/03/03  21:10:00  thor
 * Oops! Interrupt enable disable was a mistake.
 *
 * Revision 1.10  1992/03/02  20:26:54  thor
 * Fixed mistyped include name.
 *
 * Revision 1.9  1992/03/02  20:23:03  thor
 * Added code around GraphicController constructor to prevent responding to
 * nonexistant interrupts.
 *
 * Revision 1.8  1992/02/06  19:45:26  thor
 * Adde call to draw cursor in white.
 *
 * Revision 1.7  1992/01/08  16:19:54  thor
 * Added code for timeout change.
 *
 * Revision 1.6  1991/11/01  20:00:08  thor
 * Added code to set up pipe from ddp to drawing routines. Changed name of
 * GetsMouse to GetsFlags.
 *
 * Revision 1.5  1991/10/30  17:58:22  thor
 * Added .5 second delay after sending DESTROY_SELF flag to let destroy
 * complete.
 *
 * Revision 1.4  1991/10/23  20:37:38  thor
 * Added Mouse support.
 *
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
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "GeGlobal.hh"
#include "GeDraw.hh"
#include "Ddp.hh"

extern "C" {
#include "sysLib.h"
#include "rebootLib.h"
#include "tp41Lib.h"
};

#include "boxed_cross.h"

static Task *GetsFlags;

static void MouseCtrl(Task &self, Mouse &mouse);

static void DdpLoop(Task &self, Pipe &pipe);

void DrawingLoop(FAST Task &self)
{
    self.FlagsInit();

    Pipe AddrPipe(sizeof(long *),500);

    // Create agc here!

    GraphicController Agc((void *)AGC_ADDR,AGC_WIDTH,AGC_HEIGHT,AGC_MEM_WIDTH,
			  AGC_VECTOR);

    Agc.setOverlayColorMap(0);	// Draw a black cursor, so we can tell
				// if it's working!

    Point a;

    a.x = AGC_WIDTH / 2;
    a.y = AGC_HEIGHT / 2;

    Mouse mouse((void *)AGC_ADDR,a,(long *)&boxed_cross,AGC_VECTOR);

    Rodent = &mouse;

    int args[2];

    args[0] = (int)&AddrPipe;

    Task ddpTask((FUNCPTR)DdpLoop,args,1,DDP_PRI);

    args[0] = (int)&mouse;

    Task mouseTask((FUNCPTR)MouseCtrl,args,1,MOUSE_PRI);

    args[0] = (int)&Agc;
    args[1] = (int)&AddrPipe;

    Task RadialTask((FUNCPTR)RadialLoop,args,2,GRAPH_PRI,6000);
    Task HorizTask((FUNCPTR)HorizLoop,args,2,GRAPH_PRI);
    Task VertTask((FUNCPTR)VertLoop,args,2,GRAPH_PRI);

    FAST Task *currTask = &RadialTask;

    GetsFlags = currTask;

    for (;;)
      {
	  FAST unsigned int flag = self.WaitOnFlags(waitMask | REBOOT,
						    FLAGS_OR);

	  if ((flag & LOAD_ONLY)) // This is all that`s needed, since
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
		      break;
		  }
		// The following is to signal which display - fore or aft.
		currTask->SetFlags(LOAD_ONLY);
	    }
	  else
	    {
		Agc.setOverlayColorMap(0xffffff00); // Now safe to
						    // draw a white cursor.
		switch(flag)
		  {
		    case REBOOT:
		      currTask->SetFlags(DESTROY_SELF);
		      taskDelay(30);
		      reboot(BOOT_NORMAL);
		      return;
		      break;
		      
		    case STOP:
		      currTask->SetFlags(flag);
		      break;
		      
		    case START:
		      currTask->SetFlags(flag);
		      break;
		      
		    case RELOAD:
		    case RESTART_DISP:
		      currTask->SetFlags(flag);
		      break;
		      
		    case FORWARD_RADIAL:
		      if (currTask != &RadialTask)
			{
			    currTask->SetFlags(DESTROY_SELF);
			    taskDelay(30); // We sleep for .5 s to allow
			                   // other active display to
			                   // completely destroy self.
			}
		      currTask = &RadialTask;
		      GetsFlags = currTask;
		      currTask->SetFlags(flag);
		      break;
		      
		    case AFT_RADIAL:
		      if (currTask != &RadialTask)
			{
			    currTask->SetFlags(DESTROY_SELF);
			    taskDelay(30);
			}
		      currTask = &RadialTask;
		      GetsFlags = currTask;
		      currTask->SetFlags(flag);
		      break;
		      
		    case FORWARD_HORIZ:
		      if (currTask != &HorizTask)
			{
			    currTask->SetFlags(DESTROY_SELF);
			    taskDelay(30);
			}
		      currTask = &HorizTask;
		      GetsFlags = currTask;
		      currTask->SetFlags(flag);
		      break;
		      
		    case AFT_HORIZ:
		      if (currTask != &HorizTask)
			{
			    currTask->SetFlags(DESTROY_SELF);
			    taskDelay(30);
			}
		      currTask = &HorizTask;
		      GetsFlags = currTask;
		      currTask->SetFlags(flag);
		      break;
		      
		    case FORWARD_VERT:
		      if (currTask != &VertTask)
			{
			    currTask->SetFlags(DESTROY_SELF);
			    taskDelay(30);
			}
		      currTask = &VertTask;
		      GetsFlags = currTask;
		      currTask->SetFlags(flag);
		      break;
		      
		    case AFT_VERT:
		      if (currTask != &VertTask)
			{
			    currTask->SetFlags(DESTROY_SELF);
			    taskDelay(30);
			}
		      currTask = &VertTask;
		      GetsFlags = currTask;
		      currTask->SetFlags(flag);
		      break;
		      
		    case TMO_CHANGE:
		      if (currTask != &HorizTask)
			break;
		      currTask->SetFlags(flag);
		      break;
		  }
	    }
      }
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
