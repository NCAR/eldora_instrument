#ifndef MYRPCSERVER_H_
#define MYRPCSERVER_H_

#include <string>
#include "xmlrpc++/XmlRpc.h"


class MyRPCHandler {
public:
	MyRPCHandler();
	void start(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result);
	void stop(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result);
	void shutdown(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result);
	
protected:
};
#endif /*MYRPCSERVER_H_*/
