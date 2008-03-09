#ifndef UTILITIESRPC_H_
#define UTILITIESRPC_H_

#include "xmlrpc++/XmlRpc.h"
using namespace XmlRpc;

class RadarStart : public XmlRpcServerMethod {
    public:
        RadarStart(XmlRpcServer* s);

        void execute(XmlRpcValue& params,
                     XmlRpcValue& result);

        std::string help();

};

class RPCServer : public XmlRpcServer {
    public:
        /// Create an RPC server
        /// @param port The socket port to use
        RPCServer(int port);
        /// start the server
        void start();
        /// Destroy the server. 
        virtual ~RPCServer();

    protected:
        /// The port number for the socket
        int _port;

};

#endif 
