#include "RPCServer.h"
#include <iostream>

///////////////////////////////////////////////////////////////////
RPCServer::RPCServer(int port, int verbosity):
	_port(port) {
    
    XmlRpc::setVerbosity(verbosity);

    // Create the server socket on the specified port
    bindAndListen(_port);

    // Enable introspection
    enableIntrospection(true);

}

///////////////////////////////////////////////////////////////////
void
RPCServer::start() {
        
    // Wait for requests indefinitely
    work(-1.0);
    
}
///////////////////////////////////////////////////////////////////
RPCServer::~RPCServer() {
	
}

///////////////////////////////////////////////////////////////////
