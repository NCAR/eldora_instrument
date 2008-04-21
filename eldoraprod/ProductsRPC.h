#ifndef PRODUCTSRPC_H_
#define PRODUCTSRPC_H_

#include "RPCServer.h"
#include "RPCCmd.h"

/// A handler for RPC calls to the products application. 
class ProductsRPC {
public:
	/// @param port The XMLRPC port number
    ProductsRPC(int port);
	virtual ~ProductsRPC();
	
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
	RPCCmd<ProductsRPC> _startCmd;
	
	/// The stop command handler
	/// @param params There are no RPC input parameters
	/// @param result
	void stop(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result);
	/// The proxy that connects stop() to the RPC server.
	RPCCmd<ProductsRPC> _stopCmd;
	
	/// The shutdown command handler
	/// @param params There are no RPC input parameters
	/// @param result
	void shutdown(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result);
	/// The proxy that connects stop() to the RPC server.
	RPCCmd<ProductsRPC> _shutdownCmd;

	/// The status command handler.
	/// @param params There are no RPC input parameters
	/// @param result The RPC result will be an map of status strings
	void status(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result);
	/// The proxy that connects status() to the RPC server.
	RPCCmd<ProductsRPC> _statusCmd;
	
};
#endif

