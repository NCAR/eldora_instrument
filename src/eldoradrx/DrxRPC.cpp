#include "DrxRPC.h"
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <sys/time.h>
#include <time.h>

///////////////////////////////////////////////////////////////////
DrxRPC::DrxRPC(int rpcport,
               bool* startFlag,
               bool* stopFlag,
               bool* shutdownFlag,
               std::vector<unsigned long> (*bytesFunction)(int boardNum)) :
    _port(rpcport), _startFlag(startFlag), _stopFlag(stopFlag),
            _shutdownFlag(shutdownFlag), _bytesFunction(bytesFunction),
            _server(_port, 0), _startCmd(&_server,
                                         *this,
                                         "start",
                                         &DrxRPC::start),
            _stopCmd(&_server, *this, "stop", &DrxRPC::stop),
            _shutdownCmd(&_server, *this, "shutdown", &DrxRPC::shutdown),
            _paramsCmd(&_server, *this, "params", &DrxRPC::params),
            _statusCmd(&_server, *this, "status", &DrxRPC::status)
{

    struct timeval currentTimeTv;
    gettimeofday(&currentTimeTv, 0);
    _lastTime = currentTimeTv.tv_sec + currentTimeTv.tv_usec/1.0e6;

    // start the server (in its own thread)
    _server.start();
}

///////////////////////////////////////////////////////////////////
DrxRPC::~DrxRPC()
{

}

///////////////////////////////////////////////////////////////////
void DrxRPC::serverShutdown()
{
    std::cout << "RPC serverShutdown received\n";
    _server.shutdown();
}

///////////////////////////////////////////////////////////////////
void DrxRPC::serverExit()
{
    std::cout << "RPC serverExit received\n";
    _server.exit();
}

///////////////////////////////////////////////////////////////////
void DrxRPC::start(XmlRpc::XmlRpcValue& params,
                   XmlRpc::XmlRpcValue& result)
{
    std::cout << "RPC start received\n";
    *_startFlag = true;
    result = "eldoradrx received start command";
}

///////////////////////////////////////////////////////////////////
void DrxRPC::stop(XmlRpc::XmlRpcValue& params,
                  XmlRpc::XmlRpcValue& result)
{
    std::cout << "RPC stop received\n";
    *_stopFlag = true;
    result = "eldoradrx received stop command";
}

///////////////////////////////////////////////////////////////////
void DrxRPC::shutdown(XmlRpc::XmlRpcValue& params,
                      XmlRpc::XmlRpcValue& result)
{
    std::cout << "RPC shutdown received\n";
    result = "eldoradrx received shutdown command";
    *_shutdownFlag = 0;
}

///////////////////////////////////////////////////////////////////
void DrxRPC::status(XmlRpc::XmlRpcValue& params,
                    XmlRpc::XmlRpcValue& result)
{
    XmlRpc::XmlRpcValue retval;

    // get the current byte counts from the board
    std::vector<unsigned long> bytes[2];
    bytes[0] = _bytesFunction(0);
    bytes[1] = _bytesFunction(1);

    struct timeval currentTimeTv;
    gettimeofday(&currentTimeTv, 0);
    double currentTime = currentTimeTv.tv_sec + currentTimeTv.tv_usec/1.0e6;

    // calculate the rates
    double deltaT = currentTime - _lastTime;
    _lastTime = currentTime;
    std::vector<double> rates[2];
    for (unsigned int i = 0; i < 2; i++) {
        rates[i].resize(8);
        for (unsigned int j = 0; j < 8; j++) {
            rates[i][j] = bytes[i][j]/deltaT;
        }
    }

    // create return values for each
    for (unsigned int i = 0; i < 8; i++) {
        std::stringstream key0, key1;
        key0 << "board0-" << i;
        retval[key0.str()] = rates[0][i]/1.0e6;
        key1 << "board1-" << i;
        retval[key1.str()] = rates[1][i]/1.0e6;
    }

    // return it
    result = retval;
}

///////////////////////////////////////////////////////////////////
void DrxRPC::params(XmlRpc::XmlRpcValue& params,
                    XmlRpc::XmlRpcValue& result)
{
    std::cout << "RPC params received\n";

    for (int i = 0; i < params[0].size(); i++) {
        std::cout << params[0][i][0];
        std::cout << "  " << params[0][i][1] << "\n";
    }

    result = 1;
}
