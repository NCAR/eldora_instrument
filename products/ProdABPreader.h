#ifndef PRODABPREADER_INC
#define PRODABPREADER_INC

#include "DDSReader.h"
#include "ABPCollector.h"
#include "EldoraProducts.h"


class ProdABPreader: public PulseReader {
    public:
        ProdABPreader(DDSSubscriber& subscriber, std::string abpTopic, EldoraProducts& consumer);
        virtual ~ProdABPreader();
        
    protected:
        void notify();
        /// The consumer of the ABP Pulses.
        EldoraProducts& _consumer;
        /// Pulse collector for forward radar
        ABPCollector _collectorFore;
        /// Pulse collector for aft radar;
        ABPCollector _collectorAft;
        /// A vector of references to our collectors. 
        /// Will allow iteration over both of them
        std::vector<ABPCollector*> _collectors;
};
#endif
