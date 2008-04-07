#include <iostream>
#include "MyRPCHandler.h"
#include "RPCServer.h"
#include "RPCCmd.h"

int main (int argc, char** argv) {
	
	// The client that will respond to the rpc commands
	MyRPCHandler rpcCmdHandler;
	
	// The RPC server
	RPCServer server(60000, 0);
	
	// The RPC commands that the client implements
	RPCCmd<MyRPCHandler>     startCmd(&server, rpcCmdHandler, "start",    &MyRPCHandler::start);
	RPCCmd<MyRPCHandler>      stopCmd(&server, rpcCmdHandler, "stop",     &MyRPCHandler::stop);
	RPCCmd<MyRPCHandler>  shutdownCmd(&server, rpcCmdHandler, "shutdown", &MyRPCHandler::shutdown);
	
	// Start the server. It will run in its own thread.
	server.start();
	
	// loop patiently
	std::cout << "server started\n";
	while (1) {
		sleep(1);
		std::cout << "sleeping\n";
	}
	
}
