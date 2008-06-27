#include <string>
#include <iostream>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "trigmux.h"

void parseOptions(int argc,
                  char* argv[],
                  std::string& ip,
                  int& port,
                  bool& aftRadar,
                  int& channel)
{

    std::string radar;
    char choice;
    bool err = false;

    // get the options
    po::options_description descripts("Options");

    descripts.add_options() 
    ("help", "describe options") 
    ("ip", po::value<std::string>(&ip), "IP address")
    ("port", po::value<int>(&port), "Port number")
    ("radar", po::value<std::string>(&radar), "radar (forward|aft)")
    ("channel", po::value<char>(&choice), "channel 0|1|2|3|P|T")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, descripts), vm);
    po::notify(vm);

    // make sure required parameters are specified
    if (!vm.count("ip")
    || !vm.count("port")
    || !vm.count("radar")
    || !vm.count("channel") ) {
        std::cerr << descripts << "\n";
        exit(1);
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

    // decode and convert channel number to 0-13 
    // (excluding 4, 11
    switch (choice) {
    case '0':
    case '1':
    case '2':
    case '3':
        channel = choice - '0';
        break;
    case 'P':
        channel = 5;
        break;
    case 'T':
        channel = 6;
        break;
    default:
        std::cerr << "channel must be 0|1|2|3|P|T\n";
        err = true;
    }
    channel = aftRadar? (channel+7):channel;
    
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
    int channel;      // channel 0-13
    
    parseOptions(argc, argv,
              ip,
              port,
              aftRadar,
              channel);
    
    // our stepped attenuator
    TrigMux mux(ip, port, aftRadar? 1:0);
    
    mux.SetMux(channel, 2);

}
