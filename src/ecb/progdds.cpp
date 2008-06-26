#include <string>
#include <iostream>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "dds.h"

void parseOptions(int argc,
                  char* argv[],
                  std::string& ip,
                  int& port,
                  bool& aftRadar,
                  int& channel,
                  double& frequency,
                  bool& noise)
{

    std::string radar;
    bool err = false;

    // get the options
    po::options_description descripts("Options");

    descripts.add_options() 
    ("help", "describe options") 
    ("ip", po::value<std::string>(&ip), "IP address")
    ("port", po::value<int>(&port), "Port number")
    ("radar", po::value<std::string>(&radar), "radar (forward|aft)")
    ("channel", po::value<int>(&channel), "channel number 0|1|2|3|4|5")
    ("frequency", po::value<double>(&frequency), "frequency 9-10 (ghz)")
    ("noisereduction", "[enable noise reduction]")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, descripts), vm);
    po::notify(vm);

    // make sure required parameters are specified
    if (!vm.count("ip")
    || !vm.count("port")
    || !vm.count("radar")
    || !vm.count("channel")
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

    if (channel < 0 || channel > 5) {
        std::cerr << "channel must be 0|1|2|3|4|5\n";
        err = true;
    }

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
    std::string ip;   // ip address
    int port;         // ip port
    bool aftRadar;    // false if forward radar, true if aft
    int channel;      // channel number
    double frequency; // frequency, in ghz
    bool noise;       // true if noise reduction is to be enabled
    
    parseOptions(argc, argv,
              ip,
              port,
              aftRadar,
              channel,
              frequency,
              noise);
    
    // our dds
    DDS dds(ip, port, channel);
    
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

}
