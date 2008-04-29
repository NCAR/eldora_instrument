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

    // initialize the trhead constructs
    ::pthread_attr_init(&_threadAttr);
    ::pthread_attr_setdetachstate(&_threadAttr,
        _detached ? PTHREAD_CREATE_DETACHED : PTHREAD_CREATE_JOINABLE);

}

///////////////////////////////////////////////////////////////////
void
RPCServer::start() {
    ::pthread_create(&_threadId, &_threadAttr, threadRun, this);    
}

///////////////////////////////////////////////////////////////////
/* static */
void* RPCServer::threadRun(void* me) {
	RPCServer* pServer = (RPCServer*) me;
	void* result = pServer->run();
	
	return result;
}

///////////////////////////////////////////////////////////////////
void* RPCServer::run() {
	
	// Functon which actually starts the XmlRpcServer running
	
    // Wait for requests indefinitely
    work(-1.0);
    
    return 0;
}

///////////////////////////////////////////////////////////////////
RPCServer::~RPCServer() {
	
}

///////////////////////////////////////////////////////////////////
