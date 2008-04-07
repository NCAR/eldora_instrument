#include "DrxRPC.h"
#include <iostream>
#include <map>
#include <string>

///////////////////////////////////////////////////////////////////
DrxRPC::DrxRPC(int port) :
	_port(port),
	_server(_port, 0),
	_startCmd(&_server, *this, "start", &DrxRPC::start),
	_stopCmd(&_server, *this, "stop", &DrxRPC::stop),
	_shutdownCmd(&_server, *this, "shutdown", &DrxRPC::shutdown),
	_statusCmd(&_server, *this, "status", &DrxRPC::status)
	{
	
	// start the server (in its own thread)
	_server.start();
}

///////////////////////////////////////////////////////////////////
DrxRPC::~DrxRPC() {

}

///////////////////////////////////////////////////////////////////
void
DrxRPC::start(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result) {
	std::cout << "start received\n";
	result = "drx started";
}

///////////////////////////////////////////////////////////////////
void
DrxRPC::stop(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result) {
	std::cout << "stop received\n";
	result = "drx stop";	
}

///////////////////////////////////////////////////////////////////
void
DrxRPC::shutdown(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result) {
	std::cout << "shutdown received\n";
	result = "drx shutdown";
	exit(0);
}

///////////////////////////////////////////////////////////////////
void
DrxRPC::status(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result) {
	std::cout << "status received\n";
	
	XmlRpc::XmlRpcValue retval;
	retval["state"] = "running";
	retval["rate"] = 2.0e6;
	result = retval;	
}


