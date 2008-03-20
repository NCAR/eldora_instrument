#include "RPCServerTest.h"

#include "RPCServer.h"

#include "RPCStartCmd.h"
#include "RPCStopCmd.h"
#include "RPCShutdownCmd.h"

int main (int argc, char** argv) {
	
	// The client that will respond to the rpc commands
	RPCServerTest test;
	
	// The RPC server
	RPCServer server(60000, 0);
	
	// The RPC commands that the client implements
	RPCStartCmd<RPCServerTest>       startCmd(&server, test);
	RPCStopCmd<RPCServerTest>         stopCmd(&server, test);
	RPCShutdownCmd<RPCServerTest> shutdownCmd(&server, test);
	
	server.start();
	
}
