#include "RPCServer.h"

int main (int argc, char** argv) {
	
	RPCServer server(60000);
	
	RadarStart radarStart(&server);
	
	RadarStop radarStop(&server);
	
	server.start();
	
}
