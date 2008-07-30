/*
 * playback an ELDORA field format file through DDS
 */
#include <iostream>
#include <fstream>
#include <cassert>
#include <DoradeCSPD.h>
#include <DoradeFRAD.h>
#include <DoradePARM.h>
#include <DoradeRADD.h>
#include <DoradeRAWD.h>
#include <DoradeRYIB.h>
#include <DoradeVOLD.h>
#include <unistd.h>

#include <boost/program_options.hpp>

// For configuration management
#include "QtConfig.h"


// To get the DDSPublisher definition
#include "DDSPublisher.h"

// ArgvParams is used to create synthetic command line arguments
#include "ArgvParams.h"

// Catch signals so that we can shutdown gracefully, allowing the
// DDSReader and DDSWriter destructors to be called.
#include "SignalCatcher.h"

#include "EldoraPlaybackMain.h"
#include "EldoraProducts.h"

namespace po = boost::program_options;


void EldoraPlaybackMain::parseArgs( std::string& inputFileName,
                                   std::string& productsTopic,
                                   std::string& ORB,
                                   std::string& DCPS,
                                   std::string& DCPSInfoRepo,
                                   bool& dualPrt,
				   int  &DCPSDebugLevel,
				   int  &DCPSTransportDebugLevel) {

    // get the options
    po::options_description descripts("Options");
    int theDebugLevel = 0;
    int theTransportLevel = 0;

    descripts.add_options() ("help", "describe options")
    ("inputFileName", po::value<std::string>(&inputFileName), "input file")
    ("productstopic",po::value<std::string>(&productsTopic), "DDS products topic")
    ("ORB", po::value<std::string>(&ORB), "ORB service configuration file (Corba ORBSvcConf arg)")
    ("DCPS", po::value<std::string>(&DCPS), "DCPS configuration file (OpenDDS DCPSConfigFile arg)")
    ("DCPSInfoRepo", po::value<std::string>(&DCPSInfoRepo), "DCPSInfoRepo URL (OpenDDS DCPSInfoRepo arg)")
    ("dualprt", "Dual prt mode")
    ("DCPSDebugLevel", po::value<int>(&theDebugLevel), "DCPSDebugLevel ")
    ("DCPSTransportDebugLevel", po::value<int>(&theTransportLevel), 
     "DCPSTransportDebugLevel ");
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(_argc, _argv, descripts), vm);

    DCPSDebugLevel = theDebugLevel;
    DCPSTransportDebugLevel = theTransportLevel;
    
    po::notify(vm);
    if (vm.count("dualprt"))
        dualPrt = true;

    if (vm.count("help")) {
        std::cout << descripts << "\n";
        exit(1);
    }
}

///////////////////////////////////////////////////////////////////////////////
EldoraPlaybackMain::EldoraPlaybackMain(int argc,
                                       char** argv) :
    _argc(argc), _argv(argv) {
    _numProductRays = 0;
}
///////////////////////////////////////////////////////////////////////////////
EldoraPlaybackMain::~EldoraPlaybackMain()
{
}

///////////////////////////////////////////////////////////////////////////////
int EldoraPlaybackMain::run() {
    // This will be initialized to zero by SignalCatcher.
    int sigNumber = 0;
    // Catch signals with our flag
    SignalCatcher::instance()->configure(&sigNumber);

    QtConfig config("NCAR", "EldoraPlayback");
    std::string productsTopic;
    std::string ORB;
    std::string DCPS;
    std::string DCPSInfoRepo;
    std::string inputFileName;

    bool dualPrt = false;
    int DCPSDebugLevel = 0;
    int DCPSTransportDebugLevel = 0;
    int numPrtIds;

    // set up the default configuration directory path
    char* e = getenv("ELDORADIR");
    std::string EldoraDir("/conf/");
    if (e) {
        EldoraDir = e + EldoraDir;
    } else {
        std::cerr << "Environment variable ELDORADIR must be set.\n";
        exit(1);
    }

    std::string orbFile = EldoraDir + "ORBSvc.conf";
    ORB = config.getString("ORBConfigFile", orbFile);

    std::string dcpsFile = EldoraDir + "DDSClient.ini";
    DCPS = config.getString("DCPSConfigFile", dcpsFile);

    std::string dcpsInfoRepo = "iiop://archiver:50000/DCPSInfoRepo";
    DCPSInfoRepo = config.getString("DCPSInfoRepo", dcpsInfoRepo);


    productsTopic = config.getString("TopicProducts", "EldoraProducts");
    
    dualPrt = config.getBool("DualPrt", false);

    parseArgs(inputFileName, productsTopic, ORB, DCPS, DCPSInfoRepo, dualPrt, DCPSDebugLevel,
	      DCPSTransportDebugLevel);

    // determine how many prt ids we have.
    if (dualPrt) {
        numPrtIds = 2;
    } else {
        numPrtIds = 1;
    }
    std::cout << "Number of prt ids is " << numPrtIds << "\n";

    // we have to do this bit of translation since the 
    // DDS routines want arguments starting with a single dash,
    // whereas boost::program_options uses double dashes.
    ArgvParams subParams(_argv[0]);
    subParams["-ORBSvcConf"] = ORB;
    subParams["-DCPSConfigFile"] = DCPS;
    subParams["-DCPSInfoRepo"] = DCPSInfoRepo;

    if (DCPSDebugLevel > 0) 
	subParams["-DCPSDebugLevel"] = DCPSDebugLevel;
    if (DCPSTransportDebugLevel > 0) 
	subParams["-DCPSTransportDebugLevel"] = DCPSTransportDebugLevel;

    // create the publisher
    DDSPublisher publisher(subParams.argc(), subParams.argv());
    int pubStatus = publisher.status();
    if (pubStatus)
        return pubStatus;

    // create the product generator
    // specify velocity reversal for the Eldora system
    EldoraProducts prodGenerator(publisher, productsTopic, true);

    while (1) {
        std::cerr << "calling playback\n";
        prodGenerator.playback(inputFileName);
    }
    
    
    return 0;
}
