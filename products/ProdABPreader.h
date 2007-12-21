#ifndef PRODABPREADER_INC
#define PRODABPREADER_INC

#include "DDSReader.h"
#include "EldoraProducts.h"

class ProdABPreader: public PulseReader {
    public:
        ProdABPreader(DDSSubscriber& subscriber, std::string abpTopic, EldoraProducts& consumer);
        virtual ~ProdABPreader();
        
    protected:
        void notify();
        
        EldoraProducts& _consumer;
};
#endif
