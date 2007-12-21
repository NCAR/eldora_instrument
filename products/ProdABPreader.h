#ifndef PRODABPREADER_INC
#define PRODABPREADER_INC

#include "DDSReader.h"

class ProdABPreader: public PulseReader {
    public:
        ProdABPreader(DDSSubscriber& subscriber, std::string abpTopic);
        virtual ~ProdABPreader();
        
    protected:
        void notify();
};
#endif
