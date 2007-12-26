#include <iostream>
#include <boost/program_options.hpp>

// For configuration management
#include "QtConfig.h"

// To get the DDSSubscriber and Pulsereader definitions
#include "ProdABPreader.h"

// To get the DDSPublisher definition
#include "DDSPublisher.h"

// ArgvParams is used to create syntehic command line arguments
#include "ArgvParams.h"

namespace po = boost::program_options;
void parseArgs(
        int argc,
            char** argv,
            std::string& pulseTopic,
            std::string& productsTopic,
            std::string& ORB,
            std::string& DCPS) {

    // get the options
    po::options_description descripts("Options");

    descripts.add_options() ("help", "describe options") (
            "pulsetopic",
            po::value<std::string>(&pulseTopic), "DDS pulse topic")
    ("pulsetopic",po::value<std::string>(&productsTopic), "DDS products topic")
    ("ORB", po::value<std::string>(&ORB), "ORB service configuration file (Corba ORBSvcConf arg)")
    ("DCPS", po::value<std::string>(&DCPS), "DCPS configuration file (OpenDDS DCPSConfigFile arg)")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, descripts), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << descripts << "\n";
        exit(1);
    }
}

///////////////////////////////////////////////////////////////////////////////
int main(
        int argc,
            char** argv) {

    QtConfig config("NCAR", "EldoraProd");
    std::string pulseTopic;
    std::string productsTopic;
    std::string ORB;
    std::string DCPS;

    ORB
            = config.getString(
                    "ORBConfigFile",
                    "/home/eldora/eldora/conf/tcp.conf");
    DCPS = config.getString(
            "DCPSConfigFile",
            "/home/eldora/eldora/conf/consumer.ini");
    pulseTopic = config.getString("PulseTopic", "EldoraPulses");
    productsTopic = config.getString("ProductsTopic", "EldoraProducts");

    parseArgs(argc, argv, pulseTopic, productsTopic, ORB, DCPS);

    // we have to do this bit of translation since the 
    // DDS routines want arguments starting with a single dash,
    // whereas boost::program_options uses double dashes.
    ArgvParams subParams(argv[0]);
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
    ProdABPreader abpSource(subscriber, pulseTopic, prodGenerator);

    while (1) {
        sleep(10);
        int n = abpSource.numSamples();
        std::vector<int> discards = abpSource.discards();
        std::cout << "Number of samples: " << n << " (/8:)" << n/8
                << "  product pulses:" << prodGenerator.numPulses()
                << "  discards:" << discards[0] << "," << discards[1] << "\n";
    }

    return 0;
}
