#include <string>
#include <iostream>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "dds.h"
#include "sa.h"
#include "trigmux.h"

void parseOptions(int argc,
                  char* argv[],
                  std::string& ipdds,
                  std::string& ipsamux,
                  int& port,
                  bool& aftRadar,
                  int& db,
                  int& muxchan,
                  double& frequency,
                  bool& noise)
{

    std::string radar;
    bool err = false;

    // get the options
    po::options_description descripts("Options");

    descripts.add_options() 
    ("help",                                        "describe options") 
    ("ipdds",     po::value<std::string>(&ipdds),   "IP address for dds")
    ("ipsamux",   po::value<std::string>(&ipsamux), "IP address for the attenuator and mux")
    ("port",      po::value<int>(&port),            "Port number")
    ("radar",     po::value<std::string>(&radar),   "Radar (forward|aft)")
    ("db",        po::value<int>(&db),              "Attenuation 0-120 (db)")
    ("muxchan",   po::value<int>(&muxchan),         "Test pulse multiplexor channel number 0|1|2|3")
    ("frequency", po::value<double>(&frequency),    "Frequency 9-10 (ghz)")
    ("noisereduction",                              "[Enable noise reduction]")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, descripts), vm);
    po::notify(vm);

    // make sure required parameters are specified
    if (!vm.count("ipdds")
    || !vm.count("ipsamux")
    || !vm.count("port")
    || !vm.count("radar")
    || !vm.count("muxchan")
    || !vm.count("frequency") ) {
        std::cerr << descripts << "\n";
        exit(1);
    }

    // is noise reduction requested?
    noise = false;
    if (vm.count("noisereduction")) {
        noise = true;
    }

    // do some bounds checking
    if (!radar.compare("forward")) {
        aftRadar = false;
    } else {
        if (!radar.compare("aft")) {
            aftRadar = true;
        } else {
            std::cerr << "radar must be forward|aft\n";
            err = true;
        }
    }
    
    if (db < 0 || db > 120) {
        std::cerr << "db attenuation must be between 1 and 120\n";
        err = true;
    }


    if (muxchan < 0 || muxchan > 3) {
        std::cerr << "channel must be 0|1|2|3\n";
        err = true;
    }
    // The mux channels run from 0 -13
    muxchan = aftRadar? (muxchan+7):muxchan;


    if (frequency < 9.0 || frequency > 10) {
        std::cerr << "frequency must be between 9-10 ghz\n";
        err = true;
    }
    if (err || vm.count("help")) {
        std::cerr << descripts << "\n";
        exit(1);
    }
}

int main(int argc,
         char** argv)
{
    std::string ipdds;   // ip address of dds
    std::string ipsamux;    // ip address of sa and mux
    int port;               // ip port (same for both
    bool aftRadar;          // false if forward radar, true if aft
    int muxchan;            // mux channel number
    double frequency;       // frequency, in ghz
    int db;                 // attenuation in db
    bool noise;             // true if noise reduction is to be enabled
    
    parseOptions(argc, argv,
              ipdds,
              ipsamux,
              port,
              aftRadar,
              db,
              muxchan,
              frequency,
              noise);
    
    // program dds
    // channel 5 is the test pulse channel
    DDS dds(ipdds, port, 5);
    // initialize
    dds.Rfinit();
    // Calculate values to write to DDS
    // first convert to hz
    frequency = frequency*1.0e9;  
    int B = 361 - (int)(frequency/30e6);
    int T = (int)(pow(2,24)/(B+1.0)*(256.0*181/3 - frequency/703125.0) + 0.5);
    double freqParam = 6e7*(181-T/pow(2,16)*(B+1)*3/pow(2,16));
    // Program DDS units with user input values
    dds.Rfputval(freqParam, noise? 1:0);
    
    // program the attenuator
    int saCode = 0;
    if (aftRadar)
        saCode = 8;
    // our stepped attenuator
    SA sa(ipsamux, port, saCode);
    // convert db to the attenuator parameter
    int atten = sa.ChgAttenVal(db);
    sa.SetAtten(atten);
    
    // program the mux
    // our multiplexor
    /// @todo the third parameter is inconsistently documented as either
    /// a for/aft radar choice, or selection between test pulse trigger/
    /// transmit trigger. I believe it is the latter, and 0 selects the
    /// test pulse trigger.
    TrigMux mux(ipsamux, port, 0);
    mux.SetMux(muxchan, 2);

}
