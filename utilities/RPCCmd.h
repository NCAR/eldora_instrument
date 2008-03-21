#ifndef ELDORARPCCMDS_H_
#define ELDORARPCCMDS_H_

#include <string>
#include "xmlrpc++/XmlRpc.h"
using namespace XmlRpc;

template<class T> class RPCCmd : public XmlRpcServerMethod {

public:
	RPCCmd(XmlRpcServer* s, T& cmdClient, std::string cmdName,
			void (T::*funcPtr)(XmlRpcValue& params, XmlRpcValue& result), 
			std::string helpMsg = "") :
		XmlRpcServerMethod(cmdName, s), 
		_cmdClient(cmdClient), 
		_cmdName(cmdName), 
		_funcPtr(funcPtr) {
	}
	;

	void execute(XmlRpcValue& params, XmlRpcValue& result) {
		(_cmdClient.*_funcPtr)(params, result);
	}
	;

	std::string help() {
		return _helpMsg;
	}
	;

protected:
	T& _cmdClient;
	std::string _cmdName;
	void (T::*_funcPtr)(XmlRpcValue& params, XmlRpcValue& result);
	std::string _helpMsg;
};

#endif /*ELDORARPCCMDS_H_*/
