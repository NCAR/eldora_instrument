#ifndef UTILITIESRPC_H_
#define UTILITIESRPC_H_

#include "xmlrpc++/XmlRpc.h"
using namespace XmlRpc;

class RPCServer {
public:
	/// Create an RPC server
	/// @param port The socket port to use
	RPCServer(int port);
	/// Destroy the server. 
	virtual  ~RPCServer();
	
protected:
	/// The xmprpcpp server
	XmlRpcServer _xmlrpc_server;
	/// The prot number for the socket
	int _port;
	
};

#endif /*UTILITIESRPC_H_*/
