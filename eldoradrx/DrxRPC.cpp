#include "DrxRPC.h"
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <sys/time.h>
#include <time.h>

///////////////////////////////////////////////////////////////////
DrxRPC::DrxRPC(int rpcport,
               RedRapids::RR314& board0,
               RedRapids::RR314& board1) :
    _port(rpcport), _board0(board0), _board1(board1), _server(_port, 0),
            _startCmd(&_server, *this, "start", &DrxRPC::start),
            _stopCmd(&_server, *this, "stop", &DrxRPC::stop),
            _shutdownCmd(&_server, *this, "shutdown", &DrxRPC::shutdown),
            _statusCmd(&_server, *this, "status", &DrxRPC::status) {

    struct timeval currentTimeTv;
    gettimeofday(&currentTimeTv, 0);
    _lastTime = currentTimeTv.tv_sec + currentTimeTv.tv_usec/1.0e6;
    
    // start the server (in its own thread)
    _server.start();
}

///////////////////////////////////////////////////////////////////
DrxRPC::~DrxRPC() {

}

///////////////////////////////////////////////////////////////////
void DrxRPC::start(XmlRpc::XmlRpcValue& params,
                   XmlRpc::XmlRpcValue& result) {
    std::cout << "start received\n";

    std::map<std::string, std::string> state;

    XmlRpcValue::ValueStruct p0 = params[0];

    XmlRpcValue::ValueStruct::const_iterator pp;
    for (pp = p0.begin(); pp != p0.end(); pp++) {
        std::string key = pp->first;
        XmlRpcValue value = pp->second;
        state[key] = std::string(value);
    }

    for (std::map<std::string, std::string>::const_iterator i = state.begin(); i
            != state.end(); i++) {
        std::cout << i->first << ":" << i->second << "\n";
    }

    result = "drx started";
}

///////////////////////////////////////////////////////////////////
void DrxRPC::stop(XmlRpc::XmlRpcValue& params,
                  XmlRpc::XmlRpcValue& result) {
    std::cout << "stop received\n";
    result = "drx stop";
}

///////////////////////////////////////////////////////////////////
void DrxRPC::shutdown(XmlRpc::XmlRpcValue& params,
                      XmlRpc::XmlRpcValue& result) {
    std::cout << "shutdown received\n";
    result = "drx shutdown";
    exit(0);
}

///////////////////////////////////////////////////////////////////
void DrxRPC::status(XmlRpc::XmlRpcValue& params,
                    XmlRpc::XmlRpcValue& result) {
    std::cout << "status received\n";

    XmlRpc::XmlRpcValue retval;

    // get the current byte counts from the board
    std::vector<unsigned long> bytes[2];
    bytes[0] = _board0.bytes();
    bytes[1] = _board1.bytes();

    struct timeval currentTimeTv;
    gettimeofday(&currentTimeTv, 0);
    double currentTime = currentTimeTv.tv_sec + currentTimeTv.tv_usec/1.0e6;
    
    // calculate the rates
    double deltaT = currentTime - _lastTime;
    _lastTime = currentTime;
    std::vector<double> rates[2];
    for (unsigned int i = 0; i < 2; i++) {
        rates[i].resize(8);
        for (unsigned int j = 0; j < 8; j++) {
            rates[i][j] = bytes[i][j]/deltaT;
        }
    }
    
    // create return values for each
    for (unsigned int i = 0; i < 8; i++) {
        std::stringstream key0, key1;
        key0 << "board0-" << i;
        retval[key0.str()] = rates[0][i]/1.0e6;
        key1 << "board1-" << i;
        retval[key1.str()] = rates[1][i]/1.0e6;
    }
    
    

    // return it
    result = retval;
}

