#include <QObject>
#include <QMetaType>
#include <QApplication>
#include <QString>
#include <QDialog>
#include <iostream>
#include <boost/program_options.hpp>

    Q_DECLARE_METATYPE(std::vector<double>)

#include "SdrScope.h"

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
            std::string& DCPS) {

    // get the options
    po::options_description descripts("Options");

    descripts.add_options() ("help", "describe options") ("pulsetopic",
            po::value<std::string>(&pulseTopic), "DDS pulse topic")
    ("tstopic", po::value<std::string>(&tsTopic), "DDS time series topic")
    ("ORB", po::value<std::string>(&ORB), "ORB service configuration file (Corba ORBSvcConf arg)")
    ("DCPS", po::value<std::string>(&DCPS), "DCPS configuration file (OpenDDS DCPSConfigFile arg)")
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

    parseArgs(argc, argv, pulseTopic, tsTopic, ORB, DCPS);

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
    QDialog* dialog = new QDialog;

    // create our test dialog. It will contain an SdrScope
    SdrScope s(dialog);

    // create the readers
    //EldoraScopeReader<PulseReader> pulseReader(subscriber, pulseTopic, s);
    EldoraScopeReader reader(subscriber, pulseTopic);

    QObject::connect(&reader, 
    SIGNAL(
            newData(std::vector<double>,
                    std::vector<double>,
                    double,
                    double)
    ), &s, 
    SLOT(
            addDataSlot(std::vector<double>,
                    std::vector<double>,
                    double,
                    double)
    ));

    //sleep(5);

    // if we don't show() the dialog, nothing appears!
    dialog->show();

    // run the whole thing
    app.exec();

    return 0;
}
