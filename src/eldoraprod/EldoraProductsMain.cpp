#include "EldoraProductsMain.h"
#include <iostream>
#include <boost/program_options.hpp>

// For configuration management
#include "QtConfig.h"

// To get the DDSSubscriber and Rayreader definitions
#include "ProductsRayReader.h"

// To get the DDSPublisher definition
#include "DDSPublisher.h"

// ArgvParams is used to create synthetic command line arguments
#include "ArgvParams.h"

// Catch signals so that we can shutdown gracefully, allowing the
// DDSReader and DDSWriter destructors to be called.
#include "SignalCatcher.h"

namespace po = boost::program_options;
void EldoraProductsMain::parseArgs(std::string& rayTopic,
                                   std::string& productsTopic,
                                   std::string& ORB,
                                   std::string& DCPS,
                                   std::string& DCPSInfoRepo,
                                   int& nChan,
                                   bool& dualPrt,
				   int  &DCPSDebugLevel,
				   int  &DCPSTransportDebugLevel) {

    // get the options
    po::options_description descripts("Options");
    int theDebugLevel = 0;
    int theTransportLevel = 0;

    descripts.add_options() ("help", "describe options") ("raytopic",
                                                          po::value<std::string>(&rayTopic), "DDS ray topic")
    ("productstopic",po::value<std::string>(&productsTopic), "DDS products topic")
    ("ORB", po::value<std::string>(&ORB), "ORB service configuration file (Corba ORBSvcConf arg)")
    ("DCPS", po::value<std::string>(&DCPS), "DCPS configuration file (OpenDDS DCPSConfigFile arg)")
    ("DCPSInfoRepo", po::value<std::string>(&DCPSInfoRepo), "DCPSInfoRepo URL (OpenDDS DCPSInfoRepo arg)")
    ("nchan", po::value<int>(&nChan), "Number of channels containing signal (default 3)")
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
EldoraProductsMain::EldoraProductsMain(int argc,
                                       char** argv) :
    _argc(argc), _argv(argv) {
    _numAbpRays = 0;
    _numProductRays = 0;
    _numAbpDiscards.resize(2);
    _numAbpDiscards[0] = 0;
    _numAbpDiscards[1] = 0;
}

///////////////////////////////////////////////////////////////////////////////
int EldoraProductsMain::run() {
    // This will be initialized to zero by SignalCatcher.
    int sigNumber = 0;
    // Catch signals with our flag
    SignalCatcher::instance()->configure(&sigNumber);

    QtConfig config("NCAR", "EldoraProd");
    std::string rayTopic;
    std::string productsTopic;
    std::string ORB;
    std::string DCPS;
    std::string DCPSInfoRepo;
    bool dualPrt = false;
    int DCPSDebugLevel = 0;
    int DCPSTransportDebugLevel = 0;
    int nChan = 3;
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

    rayTopic = config.getString("TopicRay", "EldoraRays");
    productsTopic = config.getString("TopicProducts", "EldoraProducts");
    
    dualPrt = config.getBool("DualPrt", false);
    
    nChan = config.getInt("NumberOfChannels", 3);

    parseArgs(rayTopic, productsTopic, ORB, DCPS, DCPSInfoRepo, nChan, dualPrt, DCPSDebugLevel,
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
 // create the subscriber
    DDSSubscriber subscriber(subParams.argc(), subParams.argv());
    int subStatus = subscriber.status();
    if (subStatus)
        return subStatus;

    // create the publisher
    DDSPublisher publisher(subParams.argc(), subParams.argv());
    int pubStatus = publisher.status();
    if (pubStatus)
        return pubStatus;

    // create the product generator
    // specify velocity reversal for the Eldora system
    EldoraProducts prodGenerator(publisher, productsTopic, nChan, dualPrt, true);

    // create the abp reader. prodGenerator will 
    // receive abp data from abpSource
    ProductsRayReader abpSource(subscriber, rayTopic, prodGenerator, numPrtIds);

    while (1) {
        _numAbpRays = abpSource.numSamples();
        _numAbpDiscards = abpSource.discards();
        _numProductRays = prodGenerator.numRays();

        std::cout << "ABP rays: " << _numAbpRays 
            << "  Product rays:" << _numProductRays 
            << "  ABP discards:"
            << _numAbpDiscards[0] << "," << _numAbpDiscards[1] << "\n";
        std::cout.flush();

        for (int i = 0; i < 10; i++) {
            sleep(1);
            if (sigNumber) {
                std::cout << "Caught signal " << sigNumber << ", terminating\n";
                break;
            }
        }
        if (sigNumber)
            break;
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
EldoraProductsMain::~EldoraProductsMain() {
}

///////////////////////////////////////////////////////////////////////////////
void EldoraProductsMain::status(int& numAbpBeams,
                                int& numProductBeams,
                                int &discardBeamsFor,
                                int &discardBeamsAft) {
    numAbpBeams = _numAbpRays;
    numProductBeams = _numProductRays;
    discardBeamsFor = _numAbpDiscards[0];
    discardBeamsAft = _numAbpDiscards[1];
}

