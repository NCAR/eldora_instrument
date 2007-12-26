#ifndef PRODABPREADER_INC
#define PRODABPREADER_INC

#include "DDSReader.h"
#include "ABPCollector.h"
#include "EldoraProducts.h"

#include <vector>

class ProdABPreader: public PulseReader {
    public:
        ProdABPreader(DDSSubscriber& subscriber, std::string abpTopic, EldoraProducts& consumer);
        virtual ~ProdABPreader();
        /// @return a vector containing the the number of pulse discards. Element 0 contains
        /// fore, element 1 contains aft.
        std::vector<int> discards();
        
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
