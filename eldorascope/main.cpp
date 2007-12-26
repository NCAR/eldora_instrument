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

#include "EldoraScopeTSSource.h"
#include "EldoraScopeABPSource.h"

#include "ArgvParams.h"

namespace po = boost::program_options;
void parseArgs(
        int argc,
            char** argv,
            std::string& pulseTopic,
            std::string& tsTopic,
            std::string& ORB,
            std::string& DCPS,
            int& outputRate) {

    // get the options
    po::options_description descripts("Options");

    descripts.add_options() ("help", "describe options") ("pulsetopic",
            po::value<std::string>(&pulseTopic), "DDS pulse topic")
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

    QtConfig config("NCAR", "EldoraScope");
    std::string pulseTopic;
    std::string tsTopic;
    std::string ORB;
    std::string DCPS;
    int rate;

    rate = config.getInt("DisplayRateHz", 25);
    ORB
            = config.getString("ORBConfigFile",
                    "/home/eldora/eldora/conf/tcp.conf");
    DCPS = config.getString("DCPSConfigFile",
            "/home/eldora/eldora/conf/consumer.ini");
    tsTopic = config.getString("TSTopic", "EldoraTS");
    pulseTopic = config.getString("PulseTopic", "EldoraPulses");

    parseArgs(argc, argv, pulseTopic, tsTopic, ORB, DCPS, rate);

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

    // create the application
    QApplication app(argc, argv);

    // register our special signal data types
    qRegisterMetaType<std::vector<double> >();
    qRegisterMetaType<std::vector<int> >();
    
    // create a dialog to serve as parent for eldorascope
    QDialog* dialog = new QDialog;

    // create eldorascope
    EldoraScope scope(dialog);

    // create the readers
    EldoraScopeTSSource tsSource(subscriber, tsTopic, rate);
    EldoraScopeABPSource abpSource(subscriber, pulseTopic, rate);
    
    // connect the aboutToQuit signal from Qt to our sources,
    // so that they can shut down DDS properly.
    QObject::connect(&app, SIGNAL(aboutToQuit()), &tsSource, SLOT(shutdown()));
    QObject::connect(&app, SIGNAL(aboutToQuit()), &abpSource, SLOT(shutdown()));

    // connect the scope gate mode changes to the sources
    QObject::connect(&scope, SIGNAL(oneGateSignal(int, bool, int, int)), &tsSource, SLOT(oneGateSlot(int, bool, int,int)));
    QObject::connect(&scope, SIGNAL(oneGateSignal(int, bool, int, int)), &abpSource, SLOT(oneGateSlot(int, bool, int,int)));

    QObject::connect(&scope, SIGNAL(alongBeamSignal(int, bool)), &tsSource, SLOT(alongBeamSlot(int, bool)));
    QObject::connect(&scope, SIGNAL(alongBeamSignal(int, bool)), &abpSource, SLOT(alongBeamSlot(int, bool)));

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

    // now the ABP supply
    QObject::connect(&abpSource, 
    SIGNAL(newPData(std::vector<double>)), &scope, 
    SLOT(productSlot(std::vector<double>)));

    // if we don't show() the dialog, nothing appears!
    dialog->show();

    // note that the sources may start emitting signals
    // before the main application event loop is running,
    // in which case the signals may be lost. This could be 
    // a problem with the tsGates signalnot being captured.
    tsSource.start();
    abpSource.start();
    
    // run the whole thing
    app.exec();

    // Tell the source threads to quit.
    tsSource.quit();
    abpSource.quit();
    
    // wait for them to quit
    tsSource.wait();
    abpSource.wait();
    
    return 0;
}
