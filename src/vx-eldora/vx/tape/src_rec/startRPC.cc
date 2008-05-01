
/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 
 *	Original Author: 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1996/06/24 22:59:37  craig
 * Initial revision
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define VXWORKS
#define TAPE_CTRL_SCOPE extern

#include "vxWorks.h"
#include <errno.h>
#include <cstring>
#include "udpSvr.hh"
#include "Task.hh"
#include "Header.hh"

extern "C" {
#include "tapeStatus.h"
#include "tapeControl.h"
#include "tapeCtrlGbl.h"
void startRpc();		// So C code can call us.
};

#include "recordGbl.h"

//static TapeStatus stat;
static Task *RpcTask;
void CmdLoop(Task &);
extern "C" { void __do_global_ctors(void); };

void startRpc()
{
  // Initialize all globally declared objects.
  __do_global_ctors();
  cout.sync_with_stdio(0);

  //  tapeStatus = &stat;

  inHeader = (HeaderPtr)NULL;

  RpcTask = new Task((FUNCPTR)CmdLoop,NULL,0,2,18000,VX_FP_TASK | VX_STDIO,
		     1,"CmdLoop");
}

void CmdLoop(Task &self)
{
  udpSvr svr(3000);

  TapeCommand cmd;
  FAST TapeStatus *status = tapeStatus;

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
	}

      FAST int command = cmd.cmd;

      while (command)
	{
	  if (command == HEADER)
	    {
	      char *file = "/usr/local/vxbin/headers/current.hdr";
	      
	      int i = 0;

	      Header *Hdr = (Header *)inHeader;

	      if (Hdr == (Header *)NULL)
		{
		  Hdr = new Header(file);
		  inHeader = Hdr;
		}
	      else
		{
		  if (Hdr->readFile(file))
		    {
		      cout << "Bad open of file: " << file << endl;
		      i = 1;
		    }
		}
	      CELLSPACING *cs = Hdr->CellSpacing();
	      cout << cs->num_segments << " " << cs->spacing[0] << " " <<
		cs->num_cells[0] << endl;

	      svr.setAddr((struct sockaddr_in *)svr.getAddr());

	      svr.sendto((void *)&i,sizeof(i));
	      command = 0;
	      continue;
	    }
	  else if (command == -1)
	    command = 0;
          else if (command & STOP)
            {
	      command &= ~STOP;
	      RUN_FLAG=0;
#ifdef PRNT
	      cout << "SYSTEM STOP COMMAND RECEIVED" << endl;
#endif
            }
          else if (command & START)
            {
#ifdef PRNT
	      cout << "SYSTEM START COMMAND RECEIVED" << endl;
#endif
	      command &= ~START;
	      RUN_FLAG=1;
            }
          else if (command & REC_ON)
            {
	      command &= ~REC_ON;
	      REC_FLAG = 1;
#ifdef PRNT
	      cout << "RECORD COMMAND RECEIVED" << endl;
#endif
            }
          else if (command & REC_OFF)
            {
	      command &= ~REC_OFF;
	      REC_FLAG = 0;
#ifdef PRNT
	      cout << "RECORD OFF COMMAND RECEIVED" << endl;
#endif
            }
          else if (command & REWIND)
            {
	      command &= ~REWIND;
	      REWIND_FLAG=1;
#ifdef PRNT
	      cout << "REWIND COMMAND RECEIVED" << endl;
#endif
            }
          else if (command & INIT_UNIT)
            {
	      command &= ~INIT_UNIT;
	      //	      INIT_FLAG=1;
#ifdef PRNT
	      cout << "INIT UNIT COMMAND RECEIVED" << endl;
#endif
            }
          else if (command & USE_OTHER_UNIT)
            {
	      command &= ~USE_OTHER_UNIT;
	      //	      OTHER_UNIT_FLAG=1;
#ifdef PRNT
	      cout << "USE OTHER UNIT COMMAND RECEIVED" << endl;
#endif
            }
          else if (command & EJECT)
            {
	      command &= ~EJECT;
	      UNLOAD_FLAG=1;
#ifdef PRNT
	      cout << "EJECT COMMAND RECEIVED" << endl;
#endif
            }
	  else if (command & LOAD_TAPE)
	    {
	      command &= ~LOAD_TAPE;
	      LOAD_FLAG = 1;
	    }
	  else
	    {
	      cerr << "Unknown command " << command << "\n";
	      command = 0;
	    }
	}
      svr.setAddr((struct sockaddr_in *)svr.getAddr());

      svr.sendto((void *)status,sizeof(TapeStatus));
    }

}
