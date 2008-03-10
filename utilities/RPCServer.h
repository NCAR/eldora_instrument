#ifndef UTILITIESRPC_H_
#define UTILITIESRPC_H_

#include <string>
#include "xmlrpc++/XmlRpc.h"
using namespace XmlRpc;

class RPCMethod : public XmlRpcServerMethod {
    public:
        RPCMethod(XmlRpcServer* s, std::string name, std::string helpMsg="");

        void execute(XmlRpcValue& params,
                     XmlRpcValue& result);

        std::string help();
        
    protected:
        /// The method name
        std::string _methodName;
        /// The help message
        std::string _helpMsg;

};

class RadarStart : public XmlRpcServerMethod {
    public:
        RadarStart(XmlRpcServer* s);

        void execute(XmlRpcValue& params,
                     XmlRpcValue& result);

        std::string help();

};

class RadarStop : public XmlRpcServerMethod {
    public:
        RadarStop(XmlRpcServer* s);

        void execute(XmlRpcValue& params,
                     XmlRpcValue& result);

        std::string help();

};

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
