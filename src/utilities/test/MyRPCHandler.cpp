#include "MyRPCHandler.h"
#include <iostream>

MyRPCHandler::MyRPCHandler()
{
}

void
MyRPCHandler::start(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result) {
	
	std::cout << "I got the start command\n";
	result = "Started via a function pointer!";
	return;
			
}

void
MyRPCHandler::stop(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result) {
	
	std::cout << "I got the stop command\n";
	result = "Stopped via a function pointer!";
	return;
			
}

void
MyRPCHandler::shutdown(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result) {
	
	std::cout << "I got the shutdown command\n";
	result = "Shutdown via a function pointer!";
	return;
			
}
