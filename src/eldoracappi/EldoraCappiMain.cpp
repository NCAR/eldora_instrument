#include <QObject>
#include <QMetaType>
#include <QApplication>
#include <QString>
#include <QDialog>
#include <iostream>
#include <boost/program_options.hpp>

Q_DECLARE_METATYPE(std::vector<double>)
Q_DECLARE_METATYPE(std::vector<int>)
Q_DECLARE_METATYPE(qlonglong)

#include "EldoraTypes.h"
Q_DECLARE_METATYPE(StrMapDouble)
#include "EldoraCappi.h"
#include "SvnVersion.h"
#include "QtConfig.h"


#include "ArgvParams.h"

namespace po = boost::program_options;
void parseArgs(
	       int argc, char** argv,
	       std::string & inputFile)
{


    // get the options
    po::options_description descripts("Options");

    descripts.add_options() ("help", "describe options") 
      ("inputFile", po::value<std::string>(&inputFile), "input file ")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, descripts), vm);
    po::notify(vm);

    if (vm.count("inputFile") != 1) {
      std::cout << descripts << std::endl;
    }

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
    QtConfig config("NCAR", "EldoraCappi");
    // path to input file
    std::string inputFile;

    // set up the default configuration directory path
    char* e = getenv("ELDORADIR");
    std::string EldoraDir("/conf/");
    if (e) {
    	EldoraDir = e + EldoraDir;
    } else {
        std::cerr << "Environment variable ELDORADIR must be set.\n";
        exit(1);
    }
    

    parseArgs(argc, argv, inputFile);



    ///////////////////////////////////////////////////////////////
    //
    // Qt User Interface Components

    // create the application
    QApplication app(argc, argv);

    // register our special signal data types
    qRegisterMetaType<std::vector<double> >();
    qRegisterMetaType<std::vector<int> >();
    qRegisterMetaType<StrMapDouble>();
    qRegisterMetaType<qlonglong>();

    // create a dialog to serve as parent for eldorascope
    QDialog* dialog = new QDialog;

    // create eldoracappi
    // get our title from the coniguration
    std::string title = config.getString("title", "EldoraCappi");
    title += " ";
    title += SvnVersion::revision();
    title += "    ";
    title += inputFile;

    EldoraCappi cappi(inputFile, title, dialog);

    //QObject::connect(&cappi,
         //SIGNAL(newProductSignal(DoubleVec,int, double, StrMapDouble)),
         //                   &cappi,
         //  SLOT(productSlot(DoubleVec,int, double, StrMapDouble)) );
    

    // if we don't show() the dialog, nothing appears!
    dialog->show();

    ///////////////////////////////////////////////////////////////
    //
    // Start all of the processes

    // note that the sources may start emitting signals
    // before the main application event loop is running,
    // in which case the signals may be lost. This could be 
    // a problem with the tsGates signal not being captured.

    // run the whole thing
    app.exec();

    ///////////////////////////////////////////////////////////////
    //
    // Shutdown


    return 0;
}
