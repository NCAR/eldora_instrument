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
 *
 * Revision 1.1  1991/10/14  19:22:02  thor
 *
 *
 *
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#include "GeGlobal.hh"

static void DdpLoop(Task &self, Pipe &pipe);

void DrawingLoop(FAST Task &self)


    // Create agc here!
    
    int args[1];
    Rodent = &mouse;


    Task RadialTask((FUNCPTR)RadialLoop,args,1,GRAPH_PRI);
    Task HorizTask((FUNCPTR)HorizLoop,args,1,GRAPH_PRI);
    Task VertTask((FUNCPTR)VertLoop,args,1,GRAPH_PRI);
    Task RadialTask((FUNCPTR)RadialLoop,args,2,GRAPH_PRI,6000);
    Task HorizTask((FUNCPTR)HorizLoop,args,2,GRAPH_PRI);
    Task VertTask((FUNCPTR)VertLoop,args,2,GRAPH_PRI);

    GetsFlags = currTask;
	  FAST unsigned int flag = self.WaitOnFlags(waitMask,FLAGS_OR);
      {
	  FAST unsigned int flag = self.WaitOnFlags(waitMask | REBOOT |
						    LOAD_ONLY,
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
		  {
		      currTask->SetFlags(DESTROY_SELF);
		      taskDelay(30);
		      Task rb((FUNCPTR)Reboot,args,0);
		      return;
		      
		      currTask->SetFlags(flag);
		      break;
		      
		    case RELOAD:
		      if (currTask != &RadialTask)
			    currTask->SetFlags(DESTROY_SELF);
			    taskDelay(30); // We sleep for .5 s to allow
			                   // other active display to
			                   // completely destroy self.
		      
		      if (currTask != &RadialTask)
			{
			    currTask->SetFlags(DESTROY_SELF);
			    taskDelay(30);
		      
		      if (currTask != &HorizTask)
			{
			    currTask->SetFlags(DESTROY_SELF);
			    taskDelay(30);
		      
		      GetsFlags = currTask;
		      currTask->SetFlags(flag);
		      break;
		      
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
