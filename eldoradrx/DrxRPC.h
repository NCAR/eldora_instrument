#ifndef DRXRPC_H_
#define DRXRPC_H_

#include "RPCServer.h"
#include "RPCCmd.h"
#include "RR314.h"

/// A handler for RPC calls to the drx application. This handler
/// expects two RR314 instances to be available, and will query
/// them for status information.
class DrxRPC {
public:
	/// @param port The XMLRPC port number
	DrxRPC(int port, RedRapids::RR314& board0, RedRapids::RR314& board1);
	virtual ~DrxRPC();
	
protected:
	/// The XMLRPC port number
	int _port;
	
	/// The first RR314 board
	RedRapids::RR314& _board0;
	/// The second RR314 board
	RedRapids::RR314& _board1;
	
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
	
	/// The last time we querried the RR cards for their bytecounts.
	double _lastTime;
};
#endif /*DRXRPC_H_*/
