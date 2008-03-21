#ifndef ELDORARPCCMDS_H_
#define ELDORARPCCMDS_H_

#include <string>
#include "xmlrpc++/XmlRpc.h"
using namespace XmlRpc;

/// This class acts as a proxy between an RPC server and arbitrary 
/// handlers which implement the desired RPC methods. 
/// The RPC infrastructure is based on the XmlRpc++ 
/// library (http://xmlrpcpp.sourceforge.net/).
///
/// The handler methods which implement the RPC methods must
/// have the standard XmlRpc++ signature and return value semantics:
/// @code
/// void Handler::start(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result) {
///    int data  = params;
///    result = "Success";
/// }
/// @endcode
///
/// The implementation trick is to use a pointer
/// to the member function which will be called to handle the 
/// given command. The member function pointer is provided to the 
/// RPCCmd constructor, so that it knows where to send the RPC command.
///
/// An example of usage is:
/// @code
/// #include "XmlRpc/XmlRpc++.h"
///
/// // The RPC server
/// RPCServer server(6000, 0);
///
/// // The handler that will respond to the rpc commands
/// Handler cmdHandler;
///
/// // The command handler RPC methods are defined next:
/// RPCCmd<Handler>     startCmd(&server, cmdHandler, "start",    &Handler::start);
/// RPCCmd<Handler>      stopCmd(&server, cmdHandler, "stop",     &Handler::stop);
/// RPCCmd<Handler>  shutdownCmd(&server, cmdHandler, "shutdown", &Handler::shutdown);
///
/// // start the server
/// server.start();
/// @endcode
///
template<class T> class RPCCmd : public XmlRpcServerMethod {

public:
	/// @param s The RPC server
	/// @param cmdHandler The RPC command handler. 
	/// @param cmdName The RPC name of the method.
	/// @param funcPtr The pointer to the meber function of the handler object. Incoming RPC calls are forwarded to this method. 
	/// @param helpMsg The RPC help string, used by the RPC introspection facility.
	RPCCmd(XmlRpc::XmlRpcServer* s, T& cmdHandler, std::string cmdName, void (T::*funcPtr)(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result),
			std::string helpMsg = "") :
		XmlRpc::XmlRpcServerMethod(cmdName, s), _cmdHandler(cmdHandler),
				_cmdName(cmdName), _funcPtr(funcPtr) {
	}
	;

protected:

	/// Called by the RPC server when this RPC method is requested
	void execute(XmlRpcValue& params, XmlRpcValue& result) {
		(_cmdHandler.*_funcPtr)(params, result);
	}
	;

	/// Called by the RPC server for introspection
	std::string help() {
		return _helpMsg;
	}
	;

	/// The command handler which will receive the RPC method calls.
	T& _cmdHandler;
	/// The RPC method name
	std::string _cmdName;
	/// The _cmdHandler method to be called.
	void (T::*_funcPtr)(XmlRpcValue& params, XmlRpcValue& result);
	/// The RPC itrospection help message
	std::string _helpMsg;
};

#endif /*ELDORARPCCMDS_H_*/
