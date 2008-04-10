#include "DrxRPC.h"
#include <iostream>
#include <map>
#include <string>
#include <sstream>

///////////////////////////////////////////////////////////////////
DrxRPC::DrxRPC(int rpcport,
               RedRapids::RR314& board0,
               RedRapids::RR314& board1) :
    _port(rpcport), _board0(board0), _board1(board1), _server(_port, 0),
            _startCmd(&_server, *this, "start", &DrxRPC::start),
            _stopCmd(&_server, *this, "stop", &DrxRPC::stop),
            _shutdownCmd(&_server, *this, "shutdown", &DrxRPC::shutdown),
            _statusCmd(&_server, *this, "status", &DrxRPC::status) {

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
    std::vector<unsigned long> bytes0 = _board0.bytes();
    std::vector<unsigned long> bytes1 = _board1.bytes();

    // create return values for each
    for (unsigned int i = 0; i < bytes0.size(); i++) {
        std::stringstream key0, key1;
        key0 << "board0-" << i;
        retval[key0.str()] = (int)bytes0[i];
        key1 << "board1-" << i;
        retval[key1.str()] = (int)bytes1[i];
    }

    // return it
    result = retval;
}

