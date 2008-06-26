#include <string>
#include <iostream>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "sa.h"

void parseOptions(int argc,
                  char* argv[],
                  std::string& ip,
                  int& port,
                  bool& aftRadar,
                  int& db)
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
    ("db", po::value<int>(&db), "attenuation 1-120 (db)")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, descripts), vm);
    po::notify(vm);

    // make sure required parameters are specified
    if (!vm.count("ip")
    || !vm.count("port")
    || !vm.count("radar")
    || !vm.count("db") ) {
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

    if (db < 0 || db > 120) {
        std::cerr << "db attenuation must be between 1 and 120\n";
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
    int db;        // attenuation in db
    
    parseOptions(argc, argv,
              ip,
              port,
              aftRadar,
              db);
    
    int saCode = 0;
    if (aftRadar)
    	saCode = 8;
    // our stepped attenuator
    SA sa(ip, port, saCode);
    
    // convert db to the attenuator parameter
    int atten = sa.ChgAttenVal(db);
    
    sa.SetAtten(atten);

}
