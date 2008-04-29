#ifndef UTILITIESRPC_H_
#define UTILITIESRPC_H_

#include <string>
#include "xmlrpc++/XmlRpc.h"
#include <pthread.h>
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
        /// start the server. This function issues a pthread_create,
        /// which activates a thread starting with RPCServer::thr_run().
        /// RPCServer::thr_run() calls RPCServer::run() to actually start
        /// the XmlRpcServer.
        void start();
        /// Destroy the server. 
        virtual ~RPCServer();

    protected:
    	/// The static function that will be called in order to start the thread.
    	/// It will simply call back into our run() method.
    	/// @param me A pointer to an instance of RPCServer
    	static void* threadRun(void* me);
    	/// Functon which actually starts the XmlRpcServer running in the separate thread.
    	void* run();
        /// The port number for the socket
        int _port;
        /// The thread attributes
        pthread_attr_t _threadAttr;
        /// Set true if the thread should be detached
        bool _detached;
        /// The thread id.
        ::pthread_t _threadId;

};

#endif 
