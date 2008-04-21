#include "ProductsRPC.h"
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <sys/time.h>
#include <time.h>

///////////////////////////////////////////////////////////////////
ProductsRPC::ProductsRPC(int rpcport,
                         EldoraProductsMain& productsMain) :
    _port(rpcport), _server(_port, 0), _productsMain(productsMain),
            _startCmd(&_server, *this, "start", &ProductsRPC::start),
            _stopCmd(&_server, *this, "stop", &ProductsRPC::stop),
            _shutdownCmd(&_server, *this, "shutdown", &ProductsRPC::shutdown),
            _statusCmd(&_server, *this, "status", &ProductsRPC::status) {

    // start the server (in its own thread)
    _server.start();
}

///////////////////////////////////////////////////////////////////
ProductsRPC::~ProductsRPC() {

}

///////////////////////////////////////////////////////////////////
void ProductsRPC::start(XmlRpc::XmlRpcValue& params,
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

    result = "products started";
}

///////////////////////////////////////////////////////////////////
void ProductsRPC::stop(XmlRpc::XmlRpcValue& params,
                       XmlRpc::XmlRpcValue& result) {
    std::cout << "stop received\n";
    result = "products stop";
}

///////////////////////////////////////////////////////////////////
void ProductsRPC::shutdown(XmlRpc::XmlRpcValue& params,
                           XmlRpc::XmlRpcValue& result) {
    std::cout << "shutdown received\n";
    result = "products shutdown";
    exit(0);
}

///////////////////////////////////////////////////////////////////
void ProductsRPC::status(XmlRpc::XmlRpcValue& params,
                         XmlRpc::XmlRpcValue& result) {
    XmlRpc::XmlRpcValue retval;

    int numAbpBeams;
    int numProductBeams;
    int discardBeamsFor;
    int discardBeamsAft;

    // get the statistics from the products generator
    _productsMain.status(numAbpBeams,
                     numProductBeams,
                     discardBeamsFor,
                     discardBeamsAft);
    
    retval["numAbpBeams"] = numAbpBeams;
    retval["numProductBeams"] = numProductBeams;
    retval["discardBeamsFor"] = discardBeamsFor;
    retval["discardBeamsAft"] = discardBeamsAft;

    // return it
    result = retval;
}

