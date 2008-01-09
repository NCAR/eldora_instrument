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
#include "QtConfig.h"

// To get the DDSSubscriber definition
#include "DDSReader.h"

#include "EldoraQtProductsSource.h"

#include "ArgvParams.h"

namespace po = boost::program_options;
void parseArgs(
        int argc,
            char** argv,
            std::string& productsTopic,
            std::string& ORB,
            std::string& DCPS) {

    // get the options
    po::options_description descripts("Options");

    descripts.add_options() ("help", "describe options") (
            "productstopic",
            po::value<std::string>(&productsTopic), "DDS products topic")
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

    ///////////////////////////////////////////////////////////////
    //
    // Configuration
    
    QtConfig config("NCAR", "EldoraPPI");
    std::string productsTopic;
    std::string tsTopic;
    std::string ORB;
    std::string DCPS;

    ORB = config.getString(
                    "ORBConfigFile",
                    "/home/eldora/eldora/conf/tcp.conf");
    DCPS = config.getString(
            "DCPSConfigFile",
            "/home/eldora/eldora/conf/consumer.ini");
    productsTopic = config.getString("TopicProducts", "EldoraProducts");

    parseArgs(argc, argv, productsTopic, ORB, DCPS);

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

    // create eldorascope
    EldoraPPI ppi(dialog);

    ///////////////////////////////////////////////////////////////
    //
    // Data source Infrastructure
    
    // create the subscriber
    DDSSubscriber subscriber(subParams.argc(), subParams.argv());
    int subStatus = subscriber.status();
    if (subStatus)
        return subStatus;

    // create the reader
    EldoraQtProductsSource productsSource(subscriber, productsTopic, 0.0, true);

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
    SIGNAL(newPData(std::vector<double>, int)), &ppi, 
    SLOT(productSlot(std::vector<double>, int)));

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
