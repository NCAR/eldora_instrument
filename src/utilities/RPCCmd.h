#ifndef ELDORARPCCMDS_H_
#define ELDORARPCCMDS_H_

#include <string>
#include "XmlRpc.h"
using namespace XmlRpc;

/// This class acts as a proxy between an RPCServer and arbitrary
/// handlers which implement the desired RPC methods.
/// The RPC infrastructure is based on the XmlRpc++
/// library (http://xmlrpcpp.sourceforge.net/). Refer to the
/// XmlRpc++ documentation for details about parameter passing and
/// return value semantics.
///
/// Let's look at the command handlers first.
/// Conveniently, the handler methods which implement the RPC methods
/// use the same XmlRpc++ signature and return value semantics. An example of
/// handler methods follows:
/// @code
/// #include "xmlrpc++/XmlRpc.h"
///
/// class Handler {
/// public:
///    Handler();
///
///    void Handler::start(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result) {
///       int data  = params;
///       result = "Success";
///    }
///
///    void Handler::stop(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result) {
///       int data  = params;
///       result = 100;
///    }
/// };
/// @endcode
///
/// Now let's examine how RPCCmd is used to connect an RPCServer and the
/// handler methods. The implementation trick is to use a pointer
/// to a member function; this member function will be called to handle the
/// given command. The member function pointer is provided to the
/// RPCCmd constructor:
/// @code
/// #include "RPCServer.h"
/// #include "RPCCmd.h"
/// #include "Handler.h"
///
/// // The RPC server
/// RPCServer rpcServer(6000, 0);
///
/// // The handler that will respond to the rpc commands
/// Handler handler;
///
/// // The command handler RPC methods are defined next:
/// RPCCmd<Handler>     rpcStartCmd(&rpcServer, handler, "start",    &Handler::start);
/// RPCCmd<Handler>      rpcStopCmd(&rpcServer, handler, "stop",     &Handler::stop);
///
/// // start the XMLRPC server
/// server.start();
///
/// while (1)
///     sleep(1);
/// @endcode
///
/// Recall that RPCServer runs in its own thread, and so the RPCCmd
/// handlers will also execute in that context. They must address
/// thread synchronization issues that might exist between
/// RPC command handling and other threads of the application.
///
/// @note
/// The XmlRpcValue class provides a rich selection of assignment operators,
/// so that it can be used as a variant type. Thus you can send data types
/// that match the application needs, without having to make your own
/// conversions into and out of a single type (such as string). In fact,
/// even the data types for individual items can have different types within
/// a map, e.g.:
/// @code
/// XmlRpcValue result;
/// result["status"] = std::string("running");
/// result["counts"] = 500;
/// @endcode
/// You will want to be rational and consistent in the parameter and
/// result type usage, especially so that the python applications are
/// robust. And naturally, the parameter passing convention for each
/// RPC interface must be documented.
/// @endnote
template<class T> class RPCCmd : public XmlRpcServerMethod {

public:
	/// @param s The RPC server
	/// @param cmdHandler The RPC command handler.
	/// @param cmdName The RPC name of the method.
	/// @param funcPtr The pointer to the member function of the handler object. Incoming RPC calls are forwarded to this method.
	/// @param helpMsg The RPC help string, used by the RPC introspection facility.
	RPCCmd(XmlRpc::XmlRpcServer* s,
			T& cmdHandler, std::string cmdName,
			void (T::*funcPtr)(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result),
			std::string helpMsg = "") :
		XmlRpc::XmlRpcServerMethod(cmdName, s), _cmdHandler(cmdHandler),
				_cmdName(cmdName), _funcPtr(funcPtr) {
	}
	;

protected:

	/// Called by the RPC server when this RPC method is requested.
	/// @param params The standard XMLRPC parameters.
	/// @param result The standard XMLRPC return values.
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
