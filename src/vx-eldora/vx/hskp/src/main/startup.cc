/*
 *	$Id$
 *
 * Module: startup  
 * Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log: startup.cc,v $
 * Revision 1.13  2008/01/30 22:43:33  burghart
 * Hskp.hh is now HskpControl.h
 *
 * Revision 1.12  2008/01/29 17:37:11  burghart
 * Merged rpc/include/Header.hh into rpc/include/Header.h to create a single
 * header file to be used for both C and C++ source.
 *
 * Revision 1.11  2005/08/04 22:27:37  granger
 * commit vx/hskp as copied from /net/eldora/eldora, except a few obsolete
 * (afaik) directories were removed, like hskp/src/clock/newclk and
 * hskp/src/arinc_NCAR
 *
 * Revision 1.10  2003/10/02  15:15:21  kapoor
 * Modified to work with both TOD and Bancomm function calls
 *
 * Revision 1.9  2003/09/25  17:30:51  kapoor
 * updated for bancomm clock use - change of get_time, julian day, sync_ir
 * and start_clock
 *
 * Revision 1.8  1999/09/27  15:53:59  thor
 * Switched over to new mode of handling commands.
 *
 * Revision 1.7  1996/09/12  17:21:50  craig
 * *** empty log message ***
 *
 * Revision 1.6  1996/02/09  18:29:50  craig
 * *** empty log message ***
 *
 * Revision 1.5  1994/05/20  20:37:22  craig
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/19  17:27:19  craig
 * Initial revision
 *
 *
 * description: This module initializes RPC in Housekeeping Processor.
 *              
 */
static char rcsid[] = "$Date$ $RCSfile: startup.cc,v $ $Revision$";

#define HSKP_RPC_SCOPE

#include "cksum.h"
#include "Header.h"
#include "HskpControl.h"
#include "XmlRpc.h"

#include "vxWorks.h"
#include "stdioLib.h"
#include "taskLib.h"
#include "semLib.h"
#include "rebootLib.h"
    
#define scope extern
#define inHeader
extern "C" {
#include "hskpAll.h"
};

Header *Hdr;

static HskpStatus status;
static void startServer(void);

extern "C" { void __do_global_ctors(void); };

/*********************************************/
/* This is the start of the function startup */
/*********************************************/

void startup(void)
{
  // Initialize all globally declared objects.
  __do_global_ctors();
  cout.sync_with_stdio(0);
  cerr.sync_with_stdio(0);

  Hdr = (Header *)NULL;

  cerr << "spawning startServer" << endl;
  if(taskSpawn("CmdLoop",9,0,12000,(FUNCPTR)startServer,0,0,0,0,0,0,0,0,0,0)
     == ERROR)
    cout << "Failed to start cmd loop!!!" << endl;;
}

/**********************************************/
/* This is the start of the function startRpc */
/**********************************************/
// The RPC server
using namespace XmlRpc;
XmlRpcServer Svr;

// Start()
// Start sending housekeeping based on incoming triggers.
class StartMethod : public XmlRpcServerMethod {
public:
    StartMethod(XmlRpcServer* s) : XmlRpcServerMethod("Start", s) {}

    void execute(XmlRpcValue& params, XmlRpcValue& result)
    {
        stop_flag = 0;
        result = 0;
        std::cout << "got start command" << std::endl;
    }
} startMethod(&Svr);

// Stop()
// Stop sending housekeeping.
class StopMethod : public XmlRpcServerMethod {
public:
    StopMethod(XmlRpcServer* s) : XmlRpcServerMethod("Stop", s) {}

    void execute(XmlRpcValue& params, XmlRpcValue& result)
    {
        stop_flag = 1;
        result = 0;
        std::cout << "got start command" << std::endl;
    }
} stopMethod(&Svr);

// Header()
// Get header information from the current header file.
// On error return zero, else return the POSIX CRC-32 checksum of the
// header file cast into a signed int.
class HeaderMethod : public XmlRpcServerMethod {
public:
    HeaderMethod(XmlRpcServer* s) : XmlRpcServerMethod("Header", s) {}

    void execute(XmlRpcValue& params, XmlRpcValue& result)
    {
        std::string filename("/vxroot/headers/current.hdr");
        uint32_t crc = 0; // we use zero to indicate failure
        if (! Hdr)
            Hdr = new Header();
        if (cksum(filename, crc) != 0)
            std::cout << "Error calculating cksum for: " << filename << std::endl;
        if (Hdr->readFile(filename.c_str()) != 0)
            std::cout << "Error reading header file: " << filename << std::endl;
        
        // Return the CRC-32 checksum (cast into a signed int) as our result.
        // We hijack zero to use as a failure indicator.
        result = *(int*)&crc;
        std::cout << "Got CRC value " << crc << " for file " << 
            filename << std::endl;
    }
} headerMethod(&Svr);

static void startServer(void)
{
    currStatus = &status;
    // bind to our port and listen for commands
    Svr.bindAndListen(60001);
    // Enable introspection
    Svr.enableIntrospection(true);
    // Wait for requests indefinitely
    Svr.work(-1.0);

#if 0  
    char string[20];


  struct HskpCommand cmdBlk;

  while (1)
    {
      int bytes = svr.recvfrom((void *)&cmdBlk,sizeof(cmdBlk.cmd),MSG_PEEK);

      if ((cmdBlk.cmd == STOP) || (cmdBlk.cmd == START) ||
   (cmdBlk.cmd == HEADER) || (cmdBlk.cmd == SEND_STATUS))
 svr.recvfrom((void *)&cmdBlk,sizeof(cmdBlk.cmd));
      else
 {
   bytes = svr.recvfrom((void *)&cmdBlk,sizeof(cmdBlk));

   if ( bytes < sizeof(cmdBlk))
     {
       if (bytes < 0)
  {
    cerr << "udpSvr: " << strerror(errno) << endl;
    continue;
  }
     }
 }
      u_long cmd = cmdBlk.cmd;

      hex(cout);
      cout << "command received: " << cmd << endl;

      if (cmd == SEND_STATUS)
 cmd = 0;

      if (cmd & INIT)
 ;
      else if (cmd == HEADER)
 {
   //char *file = "/usr/local/vxbin/headers/current.hdr";
          char *file = "/vxroot/headers/current.hdr";
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
   svr.setAddr((struct sockaddr_in *)svr.getAddr());

   svr.sendto((void *)&i,sizeof(i));
   continue;
 }
      else
 {
   /***********************************************/
   /**** Start of code inserted by Craig Walther **/
   /***********************************************/
   if(cmd & START_CLOCK)  /* Has Control processor told us to
        start clock? */
     {
       clkstart = 1;
       start_clock(); 
       currStatus->clock = 0;
     }
	  if(cmd & START)  /* Has control Processor told us to start? */
	    {
	      stop_flag = 0;
	    }
	  if(cmd & STOP)   /* Has control Processor told us to stop? */
	    {
	      stop_flag = 1;
	    }
	  if(cmd & RELOAD)   /* Has control Processor told us to reload? */
	    {
       reload_flag = 1;
     }
   if(cmd & REBOOT)   /* Has control Processor told us to reboot? */
     reboot((int)BOOT_NORMAL);
   if(cmd & GPS_START)  /* Has the control processor told us to
      program the TANS II GPS receiver? */
     {
       command_gps((short)2);
     }
   if(cmd & IRIGB_SYNC) /* Has the control processor told us to sync
      the time of day card to the ADS's
      IRIG-B signal? */
     {
       
       currStatus->clock = IRIGB_SYNCING;
       char test = sync_irig();
       if(test)          /* Were we successful? */
  currStatus->clock = 0;
       else
   currStatus->clock = IRIGB_SYNC_FAILED; 
     }
   if(cmd & SET_TIME)  /* Has the Control Processor told us to set
     the time? */
     {
       char year = cmdBlk.year;
       char month = cmdBlk.month;
       char date = cmdBlk.day;
       char hour = cmdBlk.hour;
       char minute = cmdBlk.minute;
       char second = cmdBlk.second;
       set_time(hour,minute,second,month,date,year); 

             /******* This is for TOD card *******/
       /* Check for leap year, update the jday_calc array if it is *
       if((((int)(year/4))*4 == year) && (jday_calc[2] == 60))
  {
    for(int i = 2; i < 12; i++)
      jday_calc[i]++;
  }
       */

       /* Checks for leap year, updates the day of the mon array */
       if(((int)(year/4))*4 == year)
  day_mon_calc[1] = 29; 


       currStatus->clock = TIME_SET_READY;
     }
 }
      svr.setAddr((struct sockaddr_in *)svr.getAddr());

      svr.sendto((void *)currStatus,sizeof(status));
    }
#endif
}











