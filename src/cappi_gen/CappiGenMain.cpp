#include <QObject>
#include <QMetaType>
#include <QCoreApplication>
#include <QString>
#include <iostream>
#include <boost/program_options.hpp>

Q_DECLARE_METATYPE(std::vector<double>)
Q_DECLARE_METATYPE(std::vector<int>)
#include "CappiGen.h"

#include "SvnVersion.h"
#include "QtConfig.h"

// To get the DDSSubscriber definition
#include "DDSReader.h"

#include "EldoraQtProductsSource.h"

#include "ArgvParams.h"

namespace po = boost::program_options;
void parseArgs(
        int argc, char** argv, 
        std::string& productsTopic, 
        std::string& ORB,
        std::string& DCPS,
	std::string& DCPSInfoRepo,
	int &DCPSDebugLevel,
	int &DCPSTransportDebugLevel) {

    int theDebugLevel=0; 
    int theTransportLevel=0;

    // get the options
    po::options_description descripts("Options");

    descripts.add_options() ("help", "describe options") 
    ("productstopic", po::value<std::string>(&productsTopic), "DDS products topic")
    ("ORB", po::value<std::string>(&ORB), "ORB service configuration file (Corba ORBSvcConf arg)")
    ("DCPS", po::value<std::string>(&DCPS), "DCPS configuration file (OpenDDS DCPSConfigFile arg)")
    ("DCPSInfoRepo", po::value<std::string>(&DCPSInfoRepo), "DCPSInfoRepo URL (OpenDDS DCPSInfoRepo arg)")
    ("DCPSDebugLevel", po::value<int>(&theDebugLevel), "DCPSDebugLevel ")
    ("DCPSTransportDebugLevel", po::value<int>(&theTransportLevel), 
     "DCPSTransportDebugLevel")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, descripts), vm);
    po::notify(vm);

    // store parsed debug levels
    DCPSDebugLevel = theDebugLevel;
    DCPSTransportDebugLevel = theTransportLevel;

    std::cerr << argv[0] << "parse_args :DCPSDebugLevel = " << DCPSDebugLevel  << std::endl;
    std::cerr << argv[0] << "parse_args :DCPSTransportDebugLevel = " << DCPSTransportDebugLevel  << std::endl;
    if (vm.count("help")) {
        std::cout << descripts << "\n";
        exit(1);
    }

}

///////////////////////////////////////////////////////////////////////////////
int main(
        int argc, char** argv) {

    ///////////////////////////////////////////////////////////////
    //
    // Configuration

	// The EldoraPPI configuration 
    QtConfig config("NCAR", "CappiGen");
    // The products topic name
    std::string productsTopic;
    // path to the ORBSvcConf configuration file
    std::string ORB;
    // path to the DCPSConfigFile configuration file
    std::string DCPS;

    std::string DCPSInfoRepo;

    int DCPSDebugLevel = 0;
    int DCPSTransportDebugLevel = 0;

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
    ORB = config.getString("ORBConfigFile",orbFile);
    
    std::string dcpsFile = EldoraDir + "DDSClient.ini";
    DCPS = config.getString( "DCPSConfigFile", dcpsFile);

    std::string dcpsInfoRepo = "iiop://archiver:50000/DCPSInfoRepo";
    DCPSInfoRepo = config.getString("DCPSInfoRepo", dcpsInfoRepo);  

    
    productsTopic = config.getString("TopicProducts", "EldoraProducts");

    parseArgs(argc, argv, productsTopic,  ORB, DCPS, DCPSInfoRepo,
	      DCPSDebugLevel, DCPSTransportDebugLevel);

    // we have to do this bit of translation since the 
    // DDS routines want arguments starting with a single dash,
    // whereas boost::program_options uses double dashes.
    ArgvParams subParams(argv[0]);
    subParams["-ORBSvcConf"] = ORB;
    subParams["-DCPSConfigFile"] = DCPS;
    subParams["-DCPSInfoRepo"] = DCPSInfoRepo;
    if (DCPSDebugLevel > 0) {
      std::cerr << "passing DCPSDebugLevel " << DCPSDebugLevel << std::endl;
      subParams["-DCPSDebugLevel"] = DCPSDebugLevel;
    }
    if (DCPSTransportDebugLevel > 0) {
      std::cerr << "passing DCPSTransportDebugLevel " << DCPSTransportDebugLevel << std::endl;
      subParams["-DCPSTransportDebugLevel"] = DCPSTransportDebugLevel;
    }
    CappiGen cappi_gen("/tmp");


    ///////////////////////////////////////////////////////////////
    //
    // Qt User Interface Components

    // create the application
    QCoreApplication app(argc, argv);

    // register our special signal data types
    qRegisterMetaType<std::vector<double> >();
    qRegisterMetaType<std::vector<int> >();


    ///////////////////////////////////////////////////////////////
    //
    // Data source Infrastructure

    // create the subscriber
    DDSSubscriber subscriber(subParams.argc(), subParams.argv());
    int subStatus = subscriber.status();
    if (subStatus) {
      std::cerr << argv[0] << " ERROR: subscriber returned error" << subStatus << std::endl;
        return subStatus;
    }

    // create the reader
    // select the products that it will receive
    std::set<PRODUCT_TYPES> prodTypes;
    prodTypes.insert(PROD_DBZ);

    // request products from forward radar. A rate of 0.0 means send all beams
    EldoraQtProductsSource productsSource(subscriber, productsTopic, 0.0,
             EldoraQtProductsSource::RADAR_FOR , prodTypes);

    ///////////////////////////////////////////////////////////////
    //
    // Qt Signal Connections

    // connect the aboutToQuit signal from Qt to our source,
    // so that they can shut down DDS properly.
    QObject::connect(&app, SIGNAL(aboutToQuit()), &productsSource, SLOT(shutdown()));

    // now the products supply
    QObject::connect(&productsSource, 
	 SIGNAL(newPDataHskp(std::vector<double>, int, float, int, float, 
			     double, double, double, qlonglong , double , double , double, double,
			     double, double, double,double )), 
		     &cappi_gen,
		     SLOT(productSlot(std::vector<double>, 
				      int, float, int, float, double, double, double, 
				      qlonglong , double , double , double,
				      double, double, double, double, double)));


    ///////////////////////////////////////////////////////////////
    //
    // Start all of the processes

    // note that the sources may start emitting signals
    // before the main application event loop is running,
    // in which case the signals may be lost. This could be 
    // a problem with the tsGates signal not being captured.
    productsSource.start();

    // run the whole thing
    app.exec();

    ///////////////////////////////////////////////////////////////
    //
    // Shutdown

    // Tell the source threads to quit.
    productsSource.quit();

    // wait for them to quit
    productsSource.wait();

    return 0;
}
