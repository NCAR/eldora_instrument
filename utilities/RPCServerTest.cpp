#include "RPCServer.h"

int main (int argc, char** argv) {
	
	RPCServer server(60000, 5);
	
	RPCMethod radarStart(&server, "radarStart", "there is no help for you");
	
	RadarStop radarStop(&server);
	
	server.start();
	
}
