#include "EldoraProductsMain.h"
#include <iostream>
#include <boost/program_options.hpp>

// For configuration management
#include "QtConfig.h"

// To get the DDSSubscriber and Pulsereader definitions
#include "ProductsPulseReader.h"

// To get the DDSPublisher definition
#include "DDSPublisher.h"

// ArgvParams is used to create synthetic command line arguments
#include "ArgvParams.h"

// Catch signals so that we can shutdown gracefully, allowing the
// DDSReader and DDSWriter destructors to be called.
#include "SignalCatcher.h"

namespace po = boost::program_options;

void EldoraProductsMain::parseArgs(std::string& pulseTopic,
                                   std::string& productsTopic,
                                   std::string& ORB,
                                   std::string& DCPS) {

    // get the options
    po::options_description descripts("Options");

    descripts.add_options() ("help", "describe options") ("pulsetopic",
                                                          po::value<std::string>(&pulseTopic), "DDS pulse topic")
    ("productstopic",po::value<std::string>(&productsTopic), "DDS products topic")
    ("ORB", po::value<std::string>(&ORB), "ORB service configuration file (Corba ORBSvcConf arg)")
    ("DCPS", po::value<std::string>(&DCPS), "DCPS configuration file (OpenDDS DCPSConfigFile arg)")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(_argc, _argv, descripts), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << descripts << "\n";
        exit(1);
    }
}

///////////////////////////////////////////////////////////////////////////////
EldoraProductsMain::EldoraProductsMain(int argc,
                                       char** argv) :
    _argc(argc), _argv(argv) {
    _numAbpBeams = 0;
    _numProductBeams = 0;
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
    std::string pulseTopic;
    std::string productsTopic;
    std::string ORB;
    std::string DCPS;

    // set up the default configuration directory path
    char* e = getenv("ELDORADIR");
    std::string EldoraDir("/conf/");
    if (e) {
        EldoraDir = e + EldoraDir;
    }

    std::string orbFile = EldoraDir + "ORBSvc.conf";
    ORB = config.getString("ORBConfigFile", orbFile);

    std::string dcpsFile = EldoraDir + "DDSClient.ini";
    DCPS = config.getString("DCPSConfigFile", dcpsFile);

    pulseTopic = config.getString("TopicPulse", "EldoraPulses");
    productsTopic = config.getString("TopicProducts", "EldoraProducts");

    parseArgs(pulseTopic, productsTopic, ORB, DCPS);

    // we have to do this bit of translation since the 
    // DDS routines want arguments starting with a single dash,
    // whereas boost::program_options uses double dashes.
    ArgvParams subParams(_argv[0]);
    subParams["-ORBSvcConf"] = ORB;
    subParams["-DCPSConfigFile"] = DCPS;

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
    EldoraProducts prodGenerator(publisher, productsTopic);

    // create the abp reader. prodGenerator will 
    // receive abp data from abpSource
    ProductsPulseReader abpSource(subscriber, pulseTopic, prodGenerator);

    while (1) {
        _numAbpBeams = abpSource.numSamples();
        _numAbpDiscards = abpSource.discards();
        _numProductBeams = prodGenerator.numPulses();

        std::cout << "ABP beams: " << _numAbpBeams << " (/8:)" << _numAbpBeams
                /8 << "  Product beams:" << _numProductBeams << " (/2:)"
                << _numProductBeams/2 << "  ABP discards:"
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
    numAbpBeams = _numAbpBeams;
    numProductBeams = _numProductBeams;
    discardBeamsFor = _numAbpDiscards[0];
    discardBeamsAft = _numAbpDiscards[1];
}

