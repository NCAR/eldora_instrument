#ifndef DRXRPC_H_
#define DRXRPC_H_

#include "RPCServer.h"
#include "RPCCmd.h"
#include "RR314.h"
#include "EldoraRadarParams.h"

/// A handler for RPC calls to the drx application. This handler
/// expects two RR314 instances to be available, and will query
/// them for status information.
class DrxRPC
{
    public:
        /// @param port The XMLRPC port number
        /// @param startFlag Will be set true when a start command is received.
        /// @param stopFlag Will be set true when a stop command is received.
        /// @param shutdownFlag Will be set true when a shutdown command is received.
        /// @param radarParams Pointer to the radar parameters array. First entry
        /// in the array is for the forward radar parameters, second element is 
        /// for the aft radar parameters.
        /// @param The function to be called to get the byte summaries for a board
        DrxRPC(int port,
               bool* startFlag,
               bool* stopFlag,
               bool* _shutdownFlag,
               EldoraRadarParams* radarParams,
               std::vector<unsigned long> (*bytesFunction)(int boardNum));
        /// close all connections with clients and the socket file descriptor.
        void serverShutdown();
        /// Exit the server.
        void serverExit();
        /// Destructor
        virtual ~DrxRPC();

    protected:
        /// translate the values deliverd by the params RPC call
        /// into the corresponding fields in _radarParams.
        /// @param p The radar parameter structure to set.
        /// @param key The name of the parameter.
        /// @param values The values.
        /// @return True if the key was recognize and the parameter set, false otherwise.
        bool setRadarParams(EldoraRadarParams* p, std::string key,  XmlRpc::XmlRpcValue& values);
        
        /// The XMLRPC port number
        int _port;

        /// The flag to set true on start commands.
        bool* _startFlag;

        /// The flag to set true on stop commands.
        bool* _stopFlag;
        
        /// The flag to set true on shutdown commands.
        bool* _shutdownFlag;
        
        /// Pointer to the radar parameters array. First entry
        /// in the array is for the forward radar parameters, second element is 
        /// for the aft radar parameters.
        EldoraRadarParams* _radarParams;
        
        /// function to fetch byte counts
        std::vector<unsigned long> (*_bytesFunction)(int boardNum);

        /// The RPC server
        RPCServer _server;

        /// The start command handler
        /// @param params There are no RPC input parameters
        /// @param result
        void start(XmlRpc::XmlRpcValue& params,
                   XmlRpc::XmlRpcValue& result);
        /// The proxy that connects start() to the RPC server.
        RPCCmd<DrxRPC> _startCmd;

        /// The stop command handler
        /// @param params There are no RPC input parameters
        /// @param result
        void stop(XmlRpc::XmlRpcValue& params,
                  XmlRpc::XmlRpcValue& result);
        /// The proxy that connects stop() to the RPC server.
        RPCCmd<DrxRPC> _stopCmd;

        /// The shutdown command handler
        /// @param params There are no RPC input parameters
        /// @param result
        void shutdown(XmlRpc::XmlRpcValue& params,
                      XmlRpc::XmlRpcValue& result);
        /// The proxy that connects stop() to the RPC server.
        RPCCmd<DrxRPC> _shutdownCmd;

        /// Set the radar params.
        /// @param params 
        /// @param result 1 if params accepted, 0 if not.
        void params(XmlRpc::XmlRpcValue& params,
                    XmlRpc::XmlRpcValue& result);
       /// The proxy that connects params() to the RPC server.
        RPCCmd<DrxRPC> _paramsCmd;

        /// The status command handler.
        /// @param params There are no RPC input parameters
        /// @param result The RPC result will be an map of status strings
        void status(XmlRpc::XmlRpcValue& params,
                    XmlRpc::XmlRpcValue& result);
        /// The proxy that connects status() to the RPC server.
        RPCCmd<DrxRPC> _statusCmd;

         /// The last time we querried the RR cards for their bytecounts.
        double _lastTime;
};
#endif /*DRXRPC_H_*/
