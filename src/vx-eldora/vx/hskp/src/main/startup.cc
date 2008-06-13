/*
 *	$Id$
 *
 * Module: startup  
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
  currStatus = &status;

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
        std::cout << "got stop command" << std::endl;
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
        
        std::cout << "New header loaded, with cksum value " << crc << std::endl;
    }
} headerMethod(&Svr);

// status()
// Return a map of status values
class StatusMethod : public XmlRpcServerMethod {
public:
    StatusMethod(XmlRpcServer* s) : XmlRpcServerMethod("status", s) {}

    void execute(XmlRpcValue& params, XmlRpcValue& result)
    {
        XmlRpc::XmlRpcValue retval;
        retval["foreRate"] = hskpSendRate;
        retval["aftRate"] = hskpSendRate;
        // Return the map
        result = retval;
        
        std::cout << "got status command" << std::endl;
    }
} statusMethod(&Svr);

static void startServer(void)
{
    // bind to our port and listen for commands
    Svr.bindAndListen(60001);
    // Enable introspection
    Svr.enableIntrospection(true);
    // Wait for requests indefinitely
    Svr.work(-1.0);
}











