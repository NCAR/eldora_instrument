#ifndef ELDORARPCCMDS_H_
#define ELDORARPCCMDS_H_

#include <string>
#include "xmlrpc++/XmlRpc.h"
using namespace XmlRpc;

template<class T>
class RPCStartCmd: public XmlRpcServerMethod {
	
public:
	RPCStartCmd(XmlRpcServer* s, T& cmdClient, std::string helpMsg = ""):
		XmlRpcServerMethod("start", s),
		_cmdClient(cmdClient) {
	};
	
    void execute(XmlRpcValue& params,
                 XmlRpcValue& result) {
    	result = _cmdClient.start();
    };
    
    std::string help() { 
    	return _helpMsg;   	
    };
    
protected:
	T& _cmdClient;
	std::string _helpMsg;
};

/////////////////////////////////////////////////
	
template<class T>
class RPCStopCmd: public XmlRpcServerMethod {
	
public:
	RPCStopCmd(XmlRpcServer* s, T& cmdClient, std::string helpMsg = ""):
		XmlRpcServerMethod("stop", s),
		_cmdClient(cmdClient),
		_helpMsg(helpMsg){ 
	};
	
    void execute(XmlRpcValue& params,
                 XmlRpcValue& result) {
    	result = _cmdClient.stop();
    };
    
    std::string help() { 
    	return _helpMsg;   	
    };
    
protected:
	T& _cmdClient;
	std::string _helpMsg;
};

/////////////////////////////////////////////////
	
template<class T>
class RPCShutdownCmd: public XmlRpcServerMethod {
	
public:
	RPCShutdownCmd(XmlRpcServer* s, T& cmdClient, std::string helpMsg = ""):
		XmlRpcServerMethod("shutdown", s),
		_cmdClient(cmdClient) { };
	
    void execute(XmlRpcValue& params,
                 XmlRpcValue& result) {
    	result = _cmdClient.shutdown();
    };
    
    std::string help() { 
    	return _helpMsg;   	
    };
    
protected:
	T& _cmdClient;
	std::string _helpMsg;
};


#endif /*ELDORARPCCMDS_H_*/
