#ifndef RPCSERVERTEST_H_
#define RPCSERVERTEST_H_

#include <string>
#include "xmlrpc++/XmlRpc.h"


class RPCServerTest {
public:
	RPCServerTest();
	void start(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result);
	void stop(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result);
	void shutdown(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result);
	
protected:
};
#endif /*RPCSERVERTEST_H_*/
