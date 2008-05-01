/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 GeCtrl.cc
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 2.5  1996/06/21  19:32:09  thor
 * Added raw support.
 *
 * Revision 2.4  1994/09/12  18:44:18  thor
 * Changed name to senddispcommand_1_svc.
 *
 * Revision 2.3  1994/09/07  17:10:48  thor
 * Moved to new version of RPC.
 *
 * Revision 2.2  1993/09/28  13:04:33  thor
 * Added dual display support.
 *
 * Revision 2.1  1993/08/20  17:17:03  thor
 * Commented out alarm code & renamed RESTART RESTART_DISP.
 *
 * Revision 2.0  1992/11/03  12:53:30  thor
 * First offical ELDORA release!
 *
 * Revision 1.8  1992/10/21  14:33:06  thor
 * Fixed LOAD_ONLY cose to propigate command.
 *
 * Revision 1.7  1992/10/02  20:42:24  thor
 * Added LOAD_ONLY support.
 *
 * Revision 1.6  1992/01/08  16:19:29  thor
 * Added code for timeout change.
 *
 * Revision 1.5  1991/10/17  17:15:45  thor
 * Removed actual reboot from this code. Cleaned up cases.
 *
 * Revision 1.4  1991/10/17  16:46:28  thor
 * Fixed cases for RESTART & RELOAD. Corrected to only copy DispCommand if
 * a new comes in.
 *
 * Revision 1.3  1991/10/15  15:38:00  thor
 * Fixed reversed order of bcopy.
 *
 * Revision 1.2  1991/10/14  19:20:00  thor
 * Added task control calls.
 *
 * Revision 1.1  1991/10/01  16:15:11  thor
 * Initial revision
 *
 *
 *
 * description:
 *        This function runs forever, reading the UDP socket for commands.
 *  The same socket is used to reply.
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "GeGlobal.hh"
#include "vxWorks.h"
#include <errno.h>
#include <cstring>
#include "udpSvr.hh"

void svrTask(int port)
{
  udpSvr svr(port);

  DispCommand cmd;
  FAST DispStatus *status = GeStatus;

  while (1)
    {
      int bytes = svr.recvfrom((void *)&cmd,sizeof(cmd.cmd),MSG_PEEK);

      if ((cmd.cmd == STOP) || (cmd.cmd == START) || (cmd.cmd == HEADER))
	svr.recvfrom((void *)&cmd,sizeof(cmd.cmd));
      else
	{
	  bytes = svr.recvfrom((void *)&cmd,sizeof(cmd));

	  if ( bytes < sizeof(cmd))
	    {
	      if (bytes < 0)
		{
		  cerr << "udpSvr: " << strerror(errno) << endl;
		  continue;
		}
	    }
	}

      int command = cmd.cmd;

      if (command & LOAD_ONLY)
	{
	  command &= ~LOAD_ONLY;
	  memcpy((void *)GeCommand,(void *)&cmd,sizeof(DispCommand));
	  GeCommand->cmd = command; // It seems we cannot alter cmd.
#ifndef TEST_ONLY
	  DrawingTask->SetFlags(LOAD_ONLY);
#else
	  cout << "LOAD_ONLY" << endl;
#endif
	  status->status = IDLE;
	}
      else
	{
	  char *file = "/usr/local/vxbin/headers/current.hdr";
	  int i = 0;

	  switch(command)
	    {
	    case HEADER:

	      if (Hdr == (Header *)NULL)
		Hdr = new Header(file);
	      else
		{
		  if (Hdr->readFile(file))
		    {
		      cout << "Bad open of file: " << file << endl;
		      i = 1;
		    }
		}
	      status->status = IDLE;

	      svr.setAddr((struct sockaddr_in *)svr.getAddr());
	      svr.sendto((void *)&i,sizeof(i));
	      continue;
	      break;

	    case REBOOT:
	    case STOP:
#ifndef TEST_ONLY
	      DrawingTask->SetFlags(command);
#else
	  cout << "STOP" << endl;
#endif
	      status->status = IDLE;
	      break;
		
	    case START:
#ifndef TEST_ONLY
	      DrawingTask->SetFlags(command);
#else
	  cout << "START" << endl;
#endif
	      status->status = DRAWING;
	      break;
		
	    case FORWARD_RADIAL:
	      memcpy((void *)GeCommand,(void *) &cmd,sizeof(DispCommand));
#ifndef TEST_ONLY
	      DrawingTask->SetFlags(command);
#else
	  cout << "FR" << endl;
#endif
	      status->status = DRAWING;
	      break;
		
	    case FORWARD_HORIZ:
	      memcpy((void *)GeCommand,(void *) &cmd,sizeof(DispCommand));
#ifndef TEST_ONLY
	      DrawingTask->SetFlags(command);
#else
	  cout << "FH" << endl;
#endif
	      status->status = DRAWING;
	      break;
		
	    case FORWARD_VERT:
	      memcpy((void *)GeCommand,(void *) &cmd,sizeof(DispCommand));
#ifndef TEST_ONLY
	      DrawingTask->SetFlags(command);
#else
	  cout << "FV" << endl;
#endif
	      status->status = DRAWING;
	      break;
		
	    case AFT_RADIAL:
	      memcpy((void *)GeCommand,(void *) &cmd,sizeof(DispCommand));
#ifndef TEST_ONLY
	      DrawingTask->SetFlags(command);
#else
	  cout << "AR" << endl;
#endif
	      status->status = DRAWING;
	      break;
		
	    case AFT_HORIZ:
	      memcpy((void *)GeCommand,(void *) &cmd,sizeof(DispCommand));
#ifndef TEST_ONLY
	      DrawingTask->SetFlags(command);
#else
	  cout << "AH" << endl;
#endif
	      status->status = DRAWING;

	      break;
		
	    case AFT_VERT:
	      memcpy((void *)GeCommand,(void *) &cmd,sizeof(DispCommand));
#ifndef TEST_ONLY
	      DrawingTask->SetFlags(command);
#else
	  cout << "AV" << endl;
#endif
	      status->status = DRAWING;
	      break;

	    case FORWARD_DUAL:
	      memcpy((void *)GeCommand,(void *) &cmd,sizeof(DispCommand));
#ifndef TEST_ONLY
	      DrawingTask->SetFlags(command);
#else
	  cout << "FD" << endl;
#endif
	      status->status = DRAWING;
	      break;

	    case AFT_DUAL:
	      memcpy((void *)GeCommand,(void *) &cmd,sizeof(DispCommand));
#ifndef TEST_ONLY
	      DrawingTask->SetFlags(command);
#else
	  cout << "AD" << endl;
#endif
	      status->status = DRAWING;
	      break;

	    case FORWARD_RAW:
	      memcpy((void *)GeCommand,(void *) &cmd,sizeof(DispCommand));
#ifndef TEST_ONLY
	      DrawingTask->SetFlags(command);
#else
	  cout << "FR" << endl;
#endif
	      status->status = DRAWING;
	      break;

	    case AFT_RAW:
	      memcpy((void *)GeCommand,(void *) &cmd,sizeof(DispCommand));
#ifndef TEST_ONLY
	      DrawingTask->SetFlags(command);
#else
	  cout << "AR" << endl;
#endif
	      status->status = DRAWING;
	      break;

	    case TMO_CHANGE:
	      memcpy((void *)GeCommand,(void *) &cmd,sizeof(DispCommand));
#ifndef TEST_ONLY
	      DrawingTask->SetFlags(command);
#else
	  cout << "TMO" << endl;
#endif
	      break;

	    default:
	      cerr << "Unknown command received: " << cmd.cmd << endl;
	      break;
	    }
	}

      svr.setAddr((struct sockaddr_in *)svr.getAddr());

      svr.sendto((void *)&status->status,sizeof(int));
    }
}
