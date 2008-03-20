#ifndef RPCSHUTDOWNCMD_H_
#define RPCSHUTDOWNCMD_H_

#include <string>
#include "xmlrpc++/XmlRpc.h"
using namespace XmlRpc;

template<class T>
class RPCShutdownCmd: public XmlRpcServerMethod {
	
public:
	RPCShutdownCmd(XmlRpcServer* s, T& cmdRecipient):
		XmlRpcServerMethod("shutdown", s),
		_cmdRecipient(cmdRecipient) { };
	
    void execute(XmlRpcValue& params,
                 XmlRpcValue& result) {
    	result = _cmdRecipient.shutdown();
    };
    
    std::string help() { 
    	return "the shutdown command";   	
    };
    
protected:
	T& _cmdRecipient;
};

#endif /*RPCSHUTDOWNCMD_H_*/
