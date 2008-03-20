#ifndef RPCSERVERTEST_H_
#define RPCSERVERTEST_H_

#include <string>

class RPCServerTest {
public:
	RPCServerTest();
	int start();
	std::string stop();
	std::string shutdown();
	
protected:
};
#endif /*RPCSERVERTEST_H_*/
