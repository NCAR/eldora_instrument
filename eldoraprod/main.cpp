#include "EldoraProductsMain.h"
#include "ProductsRPC.h"
#include "QtConfig.h"

// simple a wrapper for EldoraProductsMain, so that the latter can be
// instantiated as a class

int main(int argc, char** argv) {
    
    // get our configuration
    QtConfig config("NCAR", "EldoraProd");
    
    // create the products generator
    EldoraProductsMain products(argc, argv);
    
    // set up the RPC interface
    int rpcport = config.getInt("RpcPort", 60002);
    ProductsRPC rpc(rpcport, products);
    
    // start the products generation
    products.run();
    
    return 0;
    
}
