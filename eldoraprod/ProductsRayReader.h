#ifndef PRODABPREADER_INC
#define PRODABPREADER_INC

#include "DDSReader.h"
#include "RayCollator.h"
#include "EldoraProducts.h"

#include <vector>

class ProductsRayReader: public RayReader {
    public:
        ProductsRayReader(DDSSubscriber& subscriber, std::string abpTopic, EldoraProducts& consumer);
        virtual ~ProductsRayReader();
        /// @return a vector containing the the number of ray discards. Element 0 contains
        /// fore, element 1 contains aft.
        std::vector<int> discards();
        
    protected:
        void notify();
        /// The consumer of the ABP Rays.
        EldoraProducts& _consumer;
        /// Ray collector for forward radar
        RayCollator _collectorFore;
        /// Ray collector for aft radar;
        RayCollator _collectorAft;
        /// A vector of references to our collectors. 
        /// Will allow iteration over both of them
        std::vector<RayCollator*> _collectors;
};
#endif
