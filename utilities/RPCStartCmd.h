#ifndef RPCSTARTCMD_H_
#define RPCSTARTCMD_H_

#include <string>
#include "xmlrpc++/XmlRpc.h"
using namespace XmlRpc;

template<class T>
class RPCStartCmd: public XmlRpcServerMethod {
	
public:
	RPCStartCmd(XmlRpcServer* s, T& cmdRecipient):
		XmlRpcServerMethod("start", s),
		_cmdRecipient(cmdRecipient) { };
	
    void execute(XmlRpcValue& params,
                 XmlRpcValue& result) {
    	result = _cmdRecipient.start();
    };
    
    std::string help() { 
    	return "the start command";   	
    };
    
protected:
	T& _cmdRecipient;
};
#endif /*RPCSTARTCMD_H_*/
