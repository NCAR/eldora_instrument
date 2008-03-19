#include "RPCServer.h"

int main (int argc, char** argv) {
	
	RPCServer server(60000, 5);
	
	RPCMethod shutdown(&server, "shutdown", "you must shutdown");
	
	RadarStart radarStart(&server);
	
	RadarStop radarStop(&server);
	
	server.start();
	
}
