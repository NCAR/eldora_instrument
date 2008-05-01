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
 * Revision 1.3  1996/06/28 21:30:11  eric
 * Modified to conform to new RPC handling.
 *
 * Revision 1.2  1994/11/14  17:24:13  eric
 * Changed to accommodate new Control Processor Software
 *
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
#include "udpSvr.hh"

#include "vxWorks.h"
#include "stdioLib.h"
#include "taskLib.h"
#include "semLib.h"
#include "rpcLib.h"
#define RDP_SCOPE
#include "RDPGlobals.h"
#include "RadarGbls.h"
#include "mcpl_gbl.h"

extern "C" {
  int rpcTaskInit(void);
  void startup();
  int init_fft_cmds_serv();
  void __do_global_ctors(void);
};

char *PN;
static RadarStatus status;
static Header *Hdr = (Header *)NULL;

static void CmdLoop();
static void fftSvr();

void startup(void)
{
  // Initialize all globally declared objects.
  __do_global_ctors();
  cout.sync_with_stdio(0);

  if(taskSpawn("fftSvr",2,0,12000,(FUNCPTR)fftSvr,0,0,0,0,0,0,0,0,0,0) == 
     ERROR)
    {
      cerr << "Failed to start FFT sever!!!\n";
      return;
    }

  if(taskSpawn("CmdLoop",2,0,12000,(FUNCPTR)CmdLoop,0,0,0,0,0,0,0,0,0,0) == 
     ERROR)
    cerr << "Failed to start command loop!!!\n";
}

static void fftSvr()
{
  if(rpcTaskInit() == ERROR)
    {
      cout << "fftSvr: Failed to start RPC code!" << endl;
      return;
    }

  /* DEFINE PROCESS NAME */
  PN = "fftserver";

  /* Register the RPC services */

  if(init_fft_cmds_serv() != 0)
    {
      cerr << PN << ": Bad fft rpc server init\n";
      cerr << "bye!" << endl;
      return;
    }
  svc_run();
}

static void CmdLoop()
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

  udpSvr svr(3000);

  RadarCommand cmd;

  while (1)
    {
      int bytes = svr.recvfrom((void *)&cmd,sizeof(cmd.cmd),MSG_PEEK);

      if ((cmd.cmd == SEND_STATUS) || (cmd.cmd == HEADER) ||
          (cmd.cmd == STOP) || (cmd.cmd == START))
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

      if (command == HEADER)
        {
          char *file = "/usr/local/vxbin/headers/current.hdr";

          int i = 0;

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
          inHeader = Hdr;

          svr.setAddr((struct sockaddr_in *)svr.getAddr());
          svr.sendto((void *)&i,sizeof(i));
          continue;
        }

      if (command == SEND_STATUS)
        command = 0;

      cs = Hdr->CellSpacing(1);
      wvfm = Hdr->Waveform();

      if(command & LOAD)
        load = 1;

      if(command & START)
        {
          
          /*        command &= !STOP; */
          /* clear out stop flag in case STOP was hit twice */
          if(!Run_flag)
            {
              semGive(exec_sem);
              Run_flag = 1;
            }
        }
      if(command & STOP)
        {
          stop = 1;
          Run_flag = 0;
        }
      if(command & TSGATE)
        {
          TS_gate = (volatile long)((cmd.dist * 1000.0 - 
                                     (double)(cs->distToFirst)) /
                                    (double)(wvfm -> gate_dist1[1]*2.5) + 0.5);
        }
      if(command & REBOOT)
        reboot = 1;
      if(command & RELOAD)
        {
          stop = 1;
          taskDelay(10); 
          /* add short delay to allow coll_data_xfer to finish */
          semGive(exec_sem);
        }
      if(command & DC_REMOVAL)
        {    
          dc_removal = 1;
          status.rp7 |= DC_BUSY;
          printf("current rp7 status = %X \n",status.rp7);
        }
      if(command & DC_1)
        ad_freq = 1;
      if(command & DC_2)
        ad_freq = 2;
      if(command & DC_3)
        ad_freq = 3;
      if(command & AD_CHAN_LO)
        ad_chan = (char )1;
      if(command & AD_CHAN_HI)
        ad_chan = (char )0;
      if(command & RESYNC)
        {    
          resync = 1;
        }
      if(command & FFT)
        fft_flag = 1;
      if(command & FFT_1)
        ts_freq = 1;
      if(command & FFT_2)
        ts_freq = 2;
      if(command & FFT_3)
        ts_freq = 3;

      if (command & 0x80000)
	{
	  status.rp7 &= ~DC_STAT;
	  status.rp7 &= ~DC_BUSY;
	}

#ifdef TASS
      if (command & TS_ON)
        {
          memcpy((char *)tass_ptr,(char *)&(cmd->tass_info),sizeof(TSACQ));
          /* Now whatever turns on time series.*/
          ts_acq_on = 1;
        }
      if (command & TS_OFF)
        ts_acq_on = 0; /* Whatever turns it off. */

#endif /* TASS */
        
//      printf("ts_freq = %d \n",ts_freq);

      svr.setAddr((struct sockaddr_in *)svr.getAddr());

      svr.sendto((void *)&status,sizeof(status));
    }

}



