#include <QObject>
#include <QMetaType>
#include <QApplication>
#include <QString>
#include <QDialog>
#include <iostream>
#include <boost/program_options.hpp>

Q_DECLARE_METATYPE(std::vector<double>)
Q_DECLARE_METATYPE(std::vector<int>)

#include "EldoraPPI.h"
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
        bool& forwardRadar,
        std::string& ORB,
        std::string& DCPS) {

    // get the options
    po::options_description descripts("Options");

    descripts.add_options() ("help", "describe options") 
    ("productstopic", po::value<std::string>(&productsTopic), "DDS products topic")
    ("forward", "View forward radar (--forward or --aft is required)")
	("aft", "View aft radar (--forward or --aft is required)")
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

    // digest the radar choice option
    if ((vm.count("forward")+vm.count("aft")) != 1) {
        std::cout << descripts << "\n";
        exit(1);    	
    }
    
    if (vm.count("forward"))
    	forwardRadar = true;
    else
    	forwardRadar = false;
    
}

///////////////////////////////////////////////////////////////////////////////
int main(
        int argc, char** argv) {

    ///////////////////////////////////////////////////////////////
    //
    // Configuration

	// The EldoraPPI configuration 
    QtConfig config("NCAR", "EldoraPPI");
    // The products topic name
    std::string productsTopic;
    // path to the ORBSvcConf configuration file
    std::string ORB;
    // path to the DCPSConfigFile configuration file
    std::string DCPS;
    // set true if viewing forward radar, false otherwise
    bool forwardRadar;

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
    
    productsTopic = config.getString("TopicProducts", "EldoraProducts");

    parseArgs(argc, argv, productsTopic, forwardRadar, ORB, DCPS);

    // we have to do this bit of translation since the 
    // DDS routines want arguments starting with a single dash,
    // whereas boost::program_options uses double dashes.
    ArgvParams subParams(argv[0]);
    subParams["-ORBSvcConf"] = ORB;
    subParams["-DCPSConfigFile"] = DCPS;

    ///////////////////////////////////////////////////////////////
    //
    // Qt User Interface Components

    // create the application
    QApplication app(argc, argv);

    // register our special signal data types
    qRegisterMetaType<std::vector<double> >();
    qRegisterMetaType<std::vector<int> >();

    // create a dialog to serve as parent for eldorascope
    QDialog* dialog = new QDialog;

    // create eldorappi
    // get our title from the coniguration
    std::string title = config.getString("title", "EldoraPPI");
    title += " ";
    title += SvnVersion::revision();
    if (forwardRadar) 
        title += " Forward Radar";
    else
        title += " Aft Radar";
    EldoraPPI ppi(title, dialog);

    ///////////////////////////////////////////////////////////////
    //
    // Data source Infrastructure

    // create the subscriber
    DDSSubscriber subscriber(subParams.argc(), subParams.argv());
    int subStatus = subscriber.status();
    if (subStatus)
        return subStatus;

    // create the reader
    // select the products that it will receive
    std::set<PRODUCT_TYPES> prodTypes;
    prodTypes.insert(PROD_DM);
    prodTypes.insert(PROD_DBZ);
    prodTypes.insert(PROD_SW);
    prodTypes.insert(PROD_NCP);
    prodTypes.insert(PROD_VR);
    prodTypes.insert(PROD_VS);
    prodTypes.insert(PROD_VL);

    // request products from both radars. A rate of 0.0 means send all beams
    EldoraQtProductsSource productsSource(subscriber, productsTopic, 0.0,
            (forwardRadar ? EldoraQtProductsSource::RADAR_FOR : EldoraQtProductsSource::RADAR_AFT), 
            prodTypes);

    ///////////////////////////////////////////////////////////////
    //
    // Qt Signal Connections

    // connect the aboutToQuit signal from Qt to our source,
    // so that they can shut down DDS properly.
    QObject::connect(&app, SIGNAL(aboutToQuit()), &productsSource, SLOT(shutdown()));

    // Connect the product type signal
    QObject::connect(&ppi, SIGNAL(alongBeamProductSignal(PRODUCT_TYPES, bool)), 
            &productsSource, SLOT(alongBeamSlot(PRODUCT_TYPES, bool)));

    // now the products supply
    QObject::connect(&productsSource, 
    SIGNAL(newPData(std::vector<double>, int, float, int, float)), &ppi, 
    SLOT(productSlot(std::vector<double>, int, float, int, float)));

    // if we don't show() the dialog, nothing appears!
    dialog->show();

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
