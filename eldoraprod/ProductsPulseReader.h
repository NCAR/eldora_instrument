#ifndef PRODABPREADER_INC
#define PRODABPREADER_INC

#include "DDSReader.h"
#include "PulseCollator.h"
#include "EldoraProducts.h"

#include <vector>

class ProductsPulseReader: public PulseReader {
    public:
        ProductsPulseReader(DDSSubscriber& subscriber, std::string abpTopic, EldoraProducts& consumer);
        virtual ~ProductsPulseReader();
        /// @return a vector containing the the number of pulse discards. Element 0 contains
        /// fore, element 1 contains aft.
        std::vector<int> discards();
        
    protected:
        void notify();
        /// The consumer of the ABP Pulses.
        EldoraProducts& _consumer;
        /// Pulse collector for forward radar
        PulseCollator _collectorFore;
        /// Pulse collector for aft radar;
        PulseCollator _collectorAft;
        /// A vector of references to our collectors. 
        /// Will allow iteration over both of them
        std::vector<PulseCollator*> _collectors;
};
#endif
