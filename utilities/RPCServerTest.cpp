#include "RPCServer.h"

int main (int argc, char** argv) {
	
	RPCServer server(60000);
	
	server.start();
	
}
