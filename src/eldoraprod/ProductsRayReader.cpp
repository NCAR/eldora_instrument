#include "ProductsRayReader.h"
#include <iostream>
/////////////////////////////////////////////////////////////////////
ProductsRayReader::ProductsRayReader(
        DDSSubscriber& subscriber,
            std::string abpTopic,
            EldoraProducts& consumer) :
    RayReader(subscriber, abpTopic), _consumer(consumer),
            _collectorFore(EldoraDDS::Forward), _collectorAft(EldoraDDS::Aft) {
    // save references to both collectors
    _collectors.push_back(&_collectorFore);
    _collectors.push_back(&_collectorAft);
}

/////////////////////////////////////////////////////////////////////
ProductsRayReader::~ProductsRayReader() {
    std::cout << __FUNCTION__ << " destructor\n";
}

/////////////////////////////////////////////////////////////////////
void ProductsRayReader::notify() {
    while (Ray* pRay = getNextItem()) {

        // give the ray to the fore or aft collector.
        // One of them should accept it. If not,
    	// we have problems (probably a corrupted
    	// radarId.
        if (!_collectorFore.addRay(pRay)) {
        	if (!_collectorAft.addRay(pRay)) {
        		// neither collector wanted the ray!
        		returnItem(pRay);
        	}
        }

        // now process any forward and aft quad rays that are available 
        for (unsigned int c = 0; c < _collectors.size(); c++) {
            RayCollator* collector = _collectors[c];

            // Do we have four rays? 
            std::vector<EldoraDDS::Ray*> rays = collector->raysReady();
            if (rays.size() == 4) {
                // Send them to the consumer.
                _consumer.newRayData(rays);
                // we are finished with the current collector crop
                for (unsigned int i = 0; i < 4; i++)
                	returnItem(rays[i]);
            }
            // return rays that the collected detected as out of sequence
            while (EldoraDDS::Ray* p = collector->finishedRay()) {
                // return the finished rays to DDS
                returnItem(p);
            }
        } // for collectors
    } // while getNextItem()
}
/////////////////////////////////////////////////////////////////////
std::vector<int> ProductsRayReader::discards() {
    std::vector<int> n;
    n.resize(2);
    n[0] = _collectorFore.discards();
    n[1] = _collectorAft.discards();
    return n;
}
