#include <QObject>
#include <QMetaType>
#include <QApplication>
#include <QString>
#include <QDialog>
#include <iostream>
#include <boost/program_options.hpp>

Q_DECLARE_METATYPE(std::vector<double>)
Q_DECLARE_METATYPE(std::vector<int>)

#include "EldoraScope.h"
#include "QtConfig.h"

// To get the DDSSubscriber definition
#include "DDSReader.h"

#include "EldoraQtTSSource.h"
#include "EldoraQtProductsSource.h"

#include "ArgvParams.h"

namespace po = boost::program_options;
void parseArgs(
        int argc,
            char** argv,
            std::string& productsTopic,
            std::string& tsTopic,
            std::string& ORB,
            std::string& DCPS,
            int& outputRate) {

    // get the options
    po::options_description descripts("Options");

    descripts.add_options() ("help", "describe options") (
            "productstopic",
            po::value<std::string>(&productsTopic), "DDS products topic")
    ("tstopic", po::value<std::string>(&tsTopic), "DDS time series topic")
    ("ORB", po::value<std::string>(&ORB), "ORB service configuration file (Corba ORBSvcConf arg)")
    ("DCPS", po::value<std::string>(&DCPS), "DCPS configuration file (OpenDDS DCPSConfigFile arg)")
    ("rate", po::value<int>(&outputRate), "Sample update rate (hz) for the display")
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
    
    QtConfig config("NCAR", "EldoraScope");
    std::string productsTopic;
    std::string tsTopic;
    std::string ORB;
    std::string DCPS;
    int rate;

    rate = config.getInt("DisplayRateHz", 25);
    ORB
            = config.getString(
                    "ORBConfigFile",
                    "/home/eldora/eldora/conf/tcp.conf");
    DCPS = config.getString(
            "DCPSConfigFile",
            "/home/eldora/eldora/conf/consumer.ini");
    tsTopic = config.getString("TopicTS", "EldoraTS");
    productsTopic = config.getString("TopicProducts", "EldoraProducts");

    parseArgs(argc, argv, productsTopic, tsTopic, ORB, DCPS, rate);

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
    EldoraScope scope(dialog);

    ///////////////////////////////////////////////////////////////
    //
    // Data source Infrastructure
    
    // create the subscriber
    DDSSubscriber subscriber(subParams.argc(), subParams.argv());
    int subStatus = subscriber.status();
    if (subStatus)
        return subStatus;

    // create the readers
    EldoraQtTSSource tsSource(subscriber, tsTopic, rate);
    EldoraQtProductsSource productsSource(subscriber, productsTopic, rate);

    ///////////////////////////////////////////////////////////////
    //
    // Qt Signal Connections
    
    // connect the aboutToQuit signal from Qt to our sources,
    // so that they can shut down DDS properly.
    QObject::connect(&app, SIGNAL(aboutToQuit()), &tsSource, SLOT(shutdown()));
    QObject::connect(&app, SIGNAL(aboutToQuit()), &productsSource, SLOT(shutdown()));

    // connect the scope gate mode changes to the sources
    QObject::connect(&scope, SIGNAL(oneGateTSSignal(int, bool, int, int)), 
            &tsSource, SLOT(oneGateSlot(int, bool, int,int)));
    QObject::connect(&scope, SIGNAL(oneGateProductSignal(PRODUCT_TYPES, bool, int, int)), 
            &productsSource, SLOT(oneGateSlot(PRODUCT_TYPES, bool, int,int)));

    QObject::connect(&scope, SIGNAL(alongBeamTSSignal(int, bool)), 
            &tsSource, SLOT(alongBeamSlot(int, bool)));
    QObject::connect(&scope, SIGNAL(alongBeamProductSignal(PRODUCT_TYPES, bool)), 
            &productsSource, SLOT(alongBeamSlot(PRODUCT_TYPES, bool)));

    // connect the gate list signal to the scope
    QObject::connect(&tsSource, SIGNAL(tsGateList(std::vector<int>)), &scope, SLOT(tsGateListSlot(std::vector<int>)));

    // now the time series data supply
    QObject::connect(&tsSource, 
    SIGNAL(newData(std::vector<double>,
                    std::vector<double>,
                    double,
                    double)), &scope, 
    SLOT(timeSeriesSlot(std::vector<double>,
                    std::vector<double>,
                    double,
                    double)));

    // now the products supply
    QObject::connect(&productsSource, 
    SIGNAL(newPData(std::vector<double>)), &scope, 
    SLOT(productSlot(std::vector<double>)));

    // if we don't show() the dialog, nothing appears!
    dialog->show();

    ///////////////////////////////////////////////////////////////
    //
    // Start all of the processes
    
    // note that the sources may start emitting signals
    // before the main application event loop is running,
    // in which case the signals may be lost. This could be 
    // a problem with the tsGates signal not being captured.
    tsSource.start();
    productsSource.start();

    // run the whole thing
    app.exec();

    ///////////////////////////////////////////////////////////////
    //
    // Shutdown
    
    // Tell the source threads to quit.
    tsSource.quit();
    productsSource.quit();

    // wait for them to quit
    tsSource.wait();
    productsSource.wait();

    return 0;
}
