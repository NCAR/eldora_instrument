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
 * Revision 2.7  1994/11/01  17:59:42  thor
 * Made converter static.
 *
 * Revision 2.6  1994/09/23  19:59:17  thor
 * Added missing setClock() for vert.
 *
 * Revision 2.5  1994/09/23  15:03:57  thor
 * Added clock code & reduced taskDelay time.
 *
 * Revision 2.4  1994/04/08  20:58:29  thor
 * Major changes!
 *
 * Revision 2.3  1993/10/27  16:04:16  thor
 * Fixed code to allow clean switch between types on the fly.
 *
 * Revision 2.2  1993/09/28  13:04:33  thor
 * Added dual display support.
 *
 * Revision 2.1  1993/08/20  17:17:03  thor
 * Cleaned up reboot action & added code to 1st draw cursor in black,
 * then white.
 *
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

void DdpLoop(Task &self, Pipe &pipe, Task &GetsFlags);

// Now that this uses 16 bit values, it's too big for the stack.
static ColorConverter conv;

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

  int args[3];

  args[0] = (int)&mouse;

  Task mouseTask((FUNCPTR)FieldMouseEvents,args,1,MOUSE_PRI,7000);

  args[0] = (int)&AddrPipe;

  FAST SEM_ID stopSem = semBCreate(SEM_Q_PRIORITY,SEM_EMPTY);

  args[1] = (int)stopSem;

  Task DisplayTask((FUNCPTR)DisplayLoop,args,2,GRAPH_PRI,10000);

  args[0] = (int)&AddrPipe;
  args[1] = (int)&DisplayTask;

  Task ddpTask((FUNCPTR)DdpLoop,args,2,DDP_PRI);

  // Now create all the display objects.
  Radial rad(&Agc);
  Dual dual(&Agc);
  Horiz horiz(&Agc);
  Vert vert(&Agc);
  Raw raw(&Agc);

  // Now create the needed shared objects.
  Clock clock(&Agc,3,0,0,1872,1024);
  clock.display();
  ParamNames namer;

  rad.setColorConverter(conv);
  rad.setClock(clock);
  rad.setParmNames(namer);

  dual.setColorConverter(conv);
  dual.setClock(clock);
  dual.setParmNames(namer);

  horiz.setColorConverter(conv);
  horiz.setClock(clock);
  horiz.setParmNames(namer);

  vert.setColorConverter(conv);
  vert.setClock(clock);
  vert.setParmNames(namer);

  raw.setClock(clock);
  raw.setcolor(30);

  for (;;)
    {
      FAST unsigned int flag = self.WaitOnFlags(mainMask,FLAGS_OR);

      Agc.setOverlayColorMap(0xffffff00); // Now safe to
      // draw a white cursor.
      if ((flag & LOAD_ONLY)) // This is all that`s needed, since
        // we don't want to start drawing yet.
        {
          FAST int cmd = (int)GeCommand->cmd;

          switch(cmd)
            {
              case FORWARD_RADIAL:
              case AFT_RADIAL:
                display = &rad;
                break;

              case FORWARD_VERT:
              case AFT_VERT:
                display = &vert;
                break;
 
              case FORWARD_HORIZ:
              case AFT_HORIZ:
                display = &horiz;
                break;

              case FORWARD_DUAL:
              case AFT_DUAL:
                display = &dual;
                break;

              case FORWARD_RAW:
              case AFT_RAW:
                display = &raw;
                break;
            }
          // The following is to signal which display - fore or aft.
          DisplayTask.SetFlags(LOAD_ONLY);
        }
      else
        {
          switch(flag)
            {
              case REBOOT:
                DisplayTask.SetFlags(UNDISPLAY);
                semTake(stopSem,30);
                reboot(BOOT_NORMAL);
                return;
                break;
		      
              case FORWARD_RADIAL:
                if (display != &rad)
                  {
                    DisplayTask.SetFlags(UNDISPLAY);
                    semTake(stopSem,WAIT_FOREVER);
                    display = &rad;
                  }
                DisplayTask.SetFlags(SHOW_FORWARD);
                break;
		      
              case AFT_RADIAL:
                if (display != &rad)
                  {
                    DisplayTask.SetFlags(UNDISPLAY);
                    semTake(stopSem,WAIT_FOREVER);
                    display = &rad;
                  }
                DisplayTask.SetFlags(SHOW_AFT);
                break;
		      
              case FORWARD_HORIZ:
                if (display != &horiz)
                  {
                    DisplayTask.SetFlags(UNDISPLAY);
                    semTake(stopSem,WAIT_FOREVER);
                    display = &horiz;
                  }
                DisplayTask.SetFlags(SHOW_FORWARD);
                break;
 		      
              case AFT_HORIZ:
                if (display != &horiz)
                  {
                    DisplayTask.SetFlags(UNDISPLAY);
                    semTake(stopSem,WAIT_FOREVER);
                    display = &horiz;
                  }
                DisplayTask.SetFlags(SHOW_AFT);
                break;
 		      
              case FORWARD_VERT:
                if (display != &vert)
                  {
                    DisplayTask.SetFlags(UNDISPLAY);
                    semTake(stopSem,WAIT_FOREVER);
                    display = &vert;
                  }
                DisplayTask.SetFlags(SHOW_FORWARD);
                break;
 		      
              case AFT_VERT:
                if (display != &vert)
                  {
                    DisplayTask.SetFlags(UNDISPLAY);
                    semTake(stopSem,WAIT_FOREVER);
                    display = &vert;
                  }
                DisplayTask.SetFlags(SHOW_AFT);
                break;
		      
              case FORWARD_DUAL:
                if (display != &dual)
                  {
                    DisplayTask.SetFlags(UNDISPLAY);
                    semTake(stopSem,WAIT_FOREVER);
                    display = &dual;
                  }
                DisplayTask.SetFlags(SHOW_FORWARD);
                break;
		      
              case AFT_DUAL:
                if (display != &dual)
                  {
                    DisplayTask.SetFlags(UNDISPLAY);
                    semTake(stopSem,WAIT_FOREVER);
                    display = &dual;
                  }
                DisplayTask.SetFlags(SHOW_AFT);
                break;

                case FORWARD_RAW:
                if (display != &raw)
                  {
                    DisplayTask.SetFlags(UNDISPLAY);
                    semTake(stopSem,WAIT_FOREVER);
                    display = &raw;
                  }
                DisplayTask.SetFlags(SHOW_FORWARD);
                break;
		      
              case AFT_RAW:
                if (display != &raw)
                  {
                    DisplayTask.SetFlags(UNDISPLAY);
                    semTake(stopSem,WAIT_FOREVER);
                    display = &raw;
                  }
                DisplayTask.SetFlags(SHOW_AFT);
                break;
                
// 		    case TMO_CHANGE:
// 		      if (display != &horiz)
// 			break;
// 		      DisplayTask.SetFlags(flag);
// 		      break;

              case START:
                DisplayTask.SetFlags(flag);
                break;
                      
              case STOP:
                DisplayTask.SetFlags(flag);
                semTake(stopSem,WAIT_FOREVER);
                break;
            }
        }
    }
}

void DdpLoop(Task &self, Pipe &pipe, FAST Task &GetsFlags)
{
    Ddp ddp((void *)DDP_ADDR,DDP_VECTOR,pipe);

    DdpCtrl = &ddp;

    for (;;)
      {
	  ddp.Next();
	  GetsFlags.SetFlags(NEW_DATA_FLAG);
      }
}
