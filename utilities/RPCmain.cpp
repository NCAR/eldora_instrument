#include "RPCServerTest.h"

#include "RPCServer.h"

#include "EldoraRpcCmds.h"

int main (int argc, char** argv) {
	
	// The client that will respond to the rpc commands
	RPCServerTest rpcCmdHandler;
	
	// The RPC server
	RPCServer server(60000, 0);
	
	// The RPC commands that the client implements
	RPCCmd<RPCServerTest>     startCmd(&server, rpcCmdHandler, "start",    &RPCServerTest::start);
	RPCCmd<RPCServerTest>      stopCmd(&server, rpcCmdHandler, "stop",     &RPCServerTest::stop);
	RPCCmd<RPCServerTest>  shutdownCmd(&server, rpcCmdHandler, "shutdown", &RPCServerTest::shutdown);
	
	server.start();
	
}
