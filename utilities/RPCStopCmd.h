#ifndef RPCSTOPCMD_H_
#define RPCSTOPCMD_H_

#include <string>
#include "xmlrpc++/XmlRpc.h"
using namespace XmlRpc;

template<class T>
class RPCStopCmd: public XmlRpcServerMethod {
	
public:
	RPCStopCmd(XmlRpcServer* s, T& cmdRecipient):
		XmlRpcServerMethod("stop", s),
		_cmdRecipient(cmdRecipient) { };
	
    void execute(XmlRpcValue& params,
                 XmlRpcValue& result) {
    	result = _cmdRecipient.stop();
    };
    
    std::string help() { 
    	return "the stop command";   	
    };
    
protected:
	T& _cmdRecipient;
};

#endif /*RPCSTOPCMD_H_*/
