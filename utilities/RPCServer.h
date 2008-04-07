#ifndef UTILITIESRPC_H_
#define UTILITIESRPC_H_

#include <string>
#include "xmlrpc++/XmlRpc.h"
using namespace XmlRpc;

/// Create an RPC server which will receive incoming XMLRPC requests.
/// The server will be initiated and run in a separate thread when start()
/// is called.
///
/// RPCCmd is utilized to provide command hanndlers that will be called
/// when RPCServer receives an incoming request. 
///
/// Since RPCServer runs in its own thread, the command handlers will also 
/// run in that thread. The command handlers must provide their own thread 
/// synchronization functionality, as needed.
class RPCServer : public XmlRpcServer {
    public:
        /// Create an RPC server
        /// @param port The socket port to use
        /// @param verbosity The debugging outpt level. 0=none
        RPCServer(int port, int verbosity=0);
        /// start the server
        void start();
        /// Destroy the server. 
        virtual ~RPCServer();

    protected:
        /// The port number for the socket
        int _port;

};

#endif 
