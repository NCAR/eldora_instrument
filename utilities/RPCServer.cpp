#include "RPCServer.h"
#include <iostream>

///////////////////////////////////////////////////////////////////
RPCMethod::RPCMethod(XmlRpcServer* s, std::string methodName, std::string helpMsg):
    XmlRpcServerMethod(methodName, s),
    _methodName(methodName),
    _helpMsg(helpMsg) {
}

///////////////////////////////////////////////////////////////////
void 
RPCMethod::execute(XmlRpcValue& params, XmlRpcValue& result)
{
    std::string resultMsg = _methodName;
    resultMsg += " executed";
    result = resultMsg;
}

///////////////////////////////////////////////////////////////////
std::string 
RPCMethod::help() { 
    return _helpMsg; 
}

///////////////////////////////////////////////////////////////////
RadarStart::RadarStart(XmlRpcServer* s):
    XmlRpcServerMethod("radarStart", s) {
}

///////////////////////////////////////////////////////////////////
void 
RadarStart::execute(XmlRpcValue& params, XmlRpcValue& result)
{
	std::cout << "params type is " << params.getType() << "  I am " << _name << "\n";
  result = "Radar started";
}

///////////////////////////////////////////////////////////////////
std::string 
RadarStart::help() { 
    return std::string("Start the radar"); 
}

///////////////////////////////////////////////////////////////////
RadarStop::RadarStop(XmlRpcServer* s):
    XmlRpcServerMethod("radarStop", s) {
}

///////////////////////////////////////////////////////////////////
void 
RadarStop::execute(XmlRpcValue& params, XmlRpcValue& result)
{
  result = "Radar stopped";
}

///////////////////////////////////////////////////////////////////
std::string 
RadarStop::help() { 
    return std::string("Stop the radar"); 
}

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
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
