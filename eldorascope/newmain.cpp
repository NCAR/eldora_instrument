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

// To get the DDSSubscriber definition
#include "DDSReader.h"
// To get the EldoraScopeReader definiton.
#include "EldoraScopeReader.h"

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
    ("rate", po::value<int>(&outputRate)->default_value(25), "Sample update rate (hz) for the display")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, descripts), vm);
    po::notify(vm);

    if (vm.count("help") || !vm.count("ORB") || !vm.count("DCPS") || !vm.count("pulsetopic") || !vm.count("tstopic")) {
        std::cout << descripts << "\n";
        exit(1);
    }
}

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {

    std::string pulseTopic;
    std::string tsTopic;
    std::string ORB;
    std::string DCPS;
    int rate;

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

    QApplication app(argc, argv);
    
    qRegisterMetaType<std::vector<double> >();
    qRegisterMetaType<std::vector<int> >();
    QDialog* dialog = new QDialog;

    // create our test dialog. It will contain an SdrScope
    EldoraScope scope(dialog);

    // create the readers
    EldoraScopeReader reader(subscriber, tsTopic, rate);

    // connect the scope gate mode changes to the reader
    QObject::connect(&scope, SIGNAL(oneGateSignal(int, int, int)),
             &reader, SLOT(oneGateSlot(int, int,int)));
     
    QObject::connect(&scope, SIGNAL(alongBeamSignal(int)),
             &reader, SLOT(alongBeamSlot(int)));
     
    // connect the reader to the scope
    
    // first the gate list
    QObject::connect(&reader, SIGNAL(gateList(std::vector<int>)),
            &scope, SLOT(gateListSlot(std::vector<int>)));
    
    // now the data supply
    QObject::connect(&reader, 
    SIGNAL(
            newData(std::vector<double>,
                    std::vector<double>,
                    double,
                    double)
    ), &scope, 
    SLOT(
            timeSeriesSlot(std::vector<double>,
                    std::vector<double>,
                    double,
                    double)
    ));

   // if we don't show() the dialog, nothing appears!
    dialog->show();

    // run the whole thing
    app.exec();

    return 0;
}
