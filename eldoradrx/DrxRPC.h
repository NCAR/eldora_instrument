#ifndef DRXRPC_H_
#define DRXRPC_H_

#include "RPCServer.h"
#include "RPCCmd.h"

/// A handler for RPC calls to the drx application
class DrxRPC {
public:
	/// @param port The XMLRPC port number
	DrxRPC(int port);
	virtual ~DrxRPC();
	
protected:
	/// The XMLRPC port number
	int _port;
	
	/// The RPC server
	RPCServer _server;
	
	/// The start command handler
	/// @param params There are no RPC input parameters
	/// @param result
	void start(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result);
	/// The proxy that connects start() to the RPC server.
	RPCCmd<DrxRPC> _startCmd;
	
	/// The stop command handler
	/// @param params There are no RPC input parameters
	/// @param result
	void stop(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result);
	/// The proxy that connects stop() to the RPC server.
	RPCCmd<DrxRPC> _stopCmd;
	
	/// The shutdown command handler
	/// @param params There are no RPC input parameters
	/// @param result
	void shutdown(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result);
	/// The proxy that connects stop() to the RPC server.
	RPCCmd<DrxRPC> _shutdownCmd;

	/// The status command handler.
	/// @param params There are no RPC input parameters
	/// @param result The RPC result will be an map of status strings
	void status(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result);
	/// The proxy that connects status() to the RPC server.
	RPCCmd<DrxRPC> _statusCmd;
	
};
#endif /*DRXRPC_H_*/
