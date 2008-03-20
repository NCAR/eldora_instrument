#include "RPCServerTest.h"
#include <iostream>

RPCServerTest::RPCServerTest()
{
}

int
RPCServerTest::start() {
	
	std::cout << "I got the start command\n";
	return 0;
			
}

std::string
RPCServerTest::stop() {
	
	std::cout << "I got the stop command\n";
	return std::string("We have been stopped!");
			
}

std::string
RPCServerTest::shutdown() {
	
	std::cout << "I got the shutdown command\n";
	return std::string("Shutdown NOW!");
			
}
