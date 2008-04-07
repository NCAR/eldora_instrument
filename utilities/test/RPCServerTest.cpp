#include "RPCServerTest.h"
#include <iostream>

RPCServerTest::RPCServerTest()
{
}

void
RPCServerTest::start(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result) {
	
	std::cout << "I got the start command\n";
	result = "Started via a function pointer!";
	return;
			
}

void
RPCServerTest::stop(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result) {
	
	std::cout << "I got the stop command\n";
	result = "Stopped via a function pointer!";
	return;
			
}

void
RPCServerTest::shutdown(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result) {
	
	std::cout << "I got the shutdown command\n";
	result = "Shutdown via a function pointer!";
	return;
			
}
