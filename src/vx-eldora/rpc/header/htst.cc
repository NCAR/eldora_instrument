#define HEADERRPC_SCOPE
#include "HeaderRpc.h"
#include <rpcLib.h>

extern "C" {
    void htst();
    void hlook();
    void __do_global_ctors();
};

void htst()
{
    __do_global_ctors();

    hex(cout);

    cout << "Hdr is at 0x" << (int)Hdr;

    dec(cout);
    
    rpcTaskInit();

    HeaderRpcInit();

    cerr << "What happened!" << endl;
}

void hlook()
{
    FAST Header *ptr = Hdr;

    TAPEHEADER *tp = ptr->GetRpcHeader();

    hex(cout);
    cout << "Tape header = 0x" << (int)tp << endl;
}
