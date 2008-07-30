#include <ArgvParams.h>
#include <QtConfig.h>
#include <xmlrpc++/XmlRpc.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "EldoraArchiver.h"

// Current write rate, in rays/second
float WriteRate = 0.0;

// The ELDORA configuration directory ($ELDORADIR/conf)
std::string EldoraConfigDir;


// Our RPC server
using namespace XmlRpc;
XmlRpcServer RpcSvr;

// Start()
// Null method, provided for convenience.
class StartMethod : public XmlRpcServerMethod {
public:
    StartMethod(XmlRpcServer* s) : XmlRpcServerMethod("Start", s) {}

    void execute(XmlRpcValue& params, XmlRpcValue& result)
    {
        result = 0;
        std::cout << "got start command" << std::endl;
    }
} startMethod(&RpcSvr);

// Stop()
// Null method, provided for convenience.
class StopMethod : public XmlRpcServerMethod {
public:
    StopMethod(XmlRpcServer* s) : XmlRpcServerMethod("Stop", s) {}

    void execute(XmlRpcValue& params, XmlRpcValue& result)
    {
        result = 0;
        std::cout << "got stop command" << std::endl;
    }
} stopMethod(&RpcSvr);

// Header()
// Load a new header.
// On error return zero, else return the POSIX CRC-32 checksum of the
// header file cast into a signed int.
class HeaderMethod : public XmlRpcServerMethod {
public:
    HeaderMethod(XmlRpcServer* s) : XmlRpcServerMethod("Header", s) {}

    void execute(XmlRpcValue& params, XmlRpcValue& result)
    {
        std::string hdrFileName(EldoraConfigDir + std::string(params[0][0]));
        std::string flightName = std::string(params[0][1]);
        EldoraArchiver::TheArchiver()->loadHeader(hdrFileName, flightName);
        // cast the unsigned checksum into a signed int
        // Return the CRC-32 checksum (cast into a signed int) as our result.
        // We hijack zero to use as a failure indicator.
        unsigned int hdrChecksum = 
            EldoraArchiver::TheArchiver()->headerChecksum();
        result = *(int*)&hdrChecksum;
    }
} headerMethod(&RpcSvr);

// status()
// Return a map of status values
class StatusMethod : public XmlRpcServerMethod {
public:
    StatusMethod(XmlRpcServer* s) : XmlRpcServerMethod("status", s) {}

    void execute(XmlRpcValue& params, XmlRpcValue& result)
    {
        XmlRpc::XmlRpcValue retval;
        retval["rate"] = WriteRate;
        // Return the CRC-32 checksum (cast into a signed int)
        unsigned int hdrChecksum = 
            EldoraArchiver::TheArchiver()->headerChecksum();
        retval["headerChecksum"] = *(int*)&hdrChecksum;
        // Return the map
        result = retval;
    }
} statusMethod(&RpcSvr);


int
main(int argc, char *argv[])
{
    const int DefaultRPCPort = 60003;
    if (argc != 1 && argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " [<data_dir> <rpc_port>]" << std::endl;
        exit(1);
    }

    std::cout << "My PID is " << getpid() << std::endl;

    // Configuration

    // get the configuration
    QtConfig eaConfig("NCAR", "EldoraArchiver");

    // set up the configuration directory path: $ELDORADIR/conf/
    char* e = getenv("ELDORADIR");
    if (! e) {
        // If ELDORADIR is not set, use a default
        e = "/opt/eldora";
        std::cerr << "Environment variable ELDORADIR is not set.  Using " <<
            e << "." << std::endl;
    }
    EldoraConfigDir = std::string(e) + "/conf/";
    
    std::string orbConfigFile = 
        eaConfig.getString("DDS/ORBConfigFile", EldoraConfigDir + "ORBSvc.conf");   
    std::string dcpsConfigFile = 
        eaConfig.getString("DDS/DCPSConfigFile", EldoraConfigDir + "DDSClient.ini");
    std::string dcpsInfoRepo = 
        eaConfig.getString("DDS/DCPSInfoRepo", "iiop://archiver:50000/DCPSInfoRepo");  
    std::string productsTopic = 
        eaConfig.getString("TopicProducts", "EldoraProducts");
    std::string dataDir =
        eaConfig.getString("DataDir", "/data_first");
    int rpcPort = 
        eaConfig.getInt("RpcPort", DefaultRPCPort);
    
    // Override dataDir and rpcPort if we got command line args
    if (argc > 1) {
        std::cout << "Using data dir from command line: " << argv[1] << std::endl;
        dataDir = std::string(argv[1]);
        std::cout << "Using RPC port from command line: " << argv[2] << std::endl;
        rpcPort = atoi(argv[2]);
    }
    
    // create the subscriber
    ArgvParams subParams(argv[0]);
    subParams["-ORBSvcConf"] = orbConfigFile;
    subParams["-DCPSConfigFile"] = dcpsConfigFile;
    subParams["-DCPSInfoRepo"] = dcpsInfoRepo;

    DDSSubscriber subscriber(subParams.argc(), subParams.argv());
    int subStatus = subscriber.status();
    if (subStatus) {
        std::cerr << "Error " << subStatus << " creating subscriber" << std::endl;
        exit(subStatus);
    }

    // Instantiate the singleton archiver
    EldoraArchiver* theArchiver = 
        EldoraArchiver::TheArchiver(subscriber, productsTopic, dataDir);

    // Initialize our RPC server
    RpcSvr.bindAndListen(rpcPort);
    RpcSvr.enableIntrospection(true);
    
    int prevRaysWritten = theArchiver->raysWritten();
    boost::posix_time::ptime lastCheckTime = 
        boost::posix_time::microsec_clock::universal_time();

    while (1) {
        // Listen for RPC commands for a bit
        RpcSvr.work(1.0);   // 1 or 2 seconds (mostly 2)...
        
        // Calculate write rate
        boost::posix_time::ptime now = 
            boost::posix_time::microsec_clock::universal_time();
        int raysWritten = theArchiver->raysWritten();
        int rayDiff = raysWritten - prevRaysWritten;
        boost::posix_time::time_duration timeDiff = now - lastCheckTime;
        WriteRate = rayDiff / (1.0e-6 * timeDiff.total_microseconds());
        
        // Save current numbers for next go-round
        prevRaysWritten = raysWritten;
        lastCheckTime = now;
    }

}
