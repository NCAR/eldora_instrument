#include "ProductsRayReader.h"
#include <iostream>
/////////////////////////////////////////////////////////////////////
ProductsRayReader::ProductsRayReader(DDSSubscriber& subscriber,
                                     std::string abpTopic,
                                     EldoraProducts& consumer,
                                     int numPrtIds) :
    RayReader(subscriber, abpTopic), _consumer(consumer),
            _collatorFor(EldoraDDS::Forward, numPrtIds),
            _collatorAft(EldoraDDS::Aft, numPrtIds),
            _numPrtIds(numPrtIds)
{
    // save references to both collectors
    _collectors.push_back(&_collatorFor);
    _collectors.push_back(&_collatorAft);
}

/////////////////////////////////////////////////////////////////////
ProductsRayReader::~ProductsRayReader()
{
    std::cout << __FUNCTION__ << " destructor\n";
}

/////////////////////////////////////////////////////////////////////
void ProductsRayReader::notify()
{
    while (Ray* pRay = getNextItem()) {
        // this diagnostic can be very useful at times.
        //std::cout << " ray num " << pRay->hskp.rayNum 
        //<< " radar:" << pRay->radarId 
        //<< " prtId:" << pRay->prtId 
        //<< " chan:" << pRay->hskp.chan <<  "\n";

        // give the ray to the fore or aft collector.
        // One of them should accept it. If not,
        // we have problems (probably a corrupted
        // radarId.
        
        if (!_collatorFor.addRay(pRay)) {
            if (!_collatorAft.addRay(pRay)) {
                // neither collector wanted the ray!
                ///@todo Need to maintain and report an "unwanted" ray count
                std::cout << "no collator wanted this ray\n";
               returnItem(pRay);
            }
        }

        // now process any forward and aft quad rays that are available 
        for (unsigned int c = 0; c < _collectors.size(); c++) {
            RayCollator* collector = _collectors[c];

            std::vector< std::vector<EldoraDDS::Ray*> > rays =
                    collector->raysReady();
            int numRays = 0;
            // Sanity check: make sure we got the expeced number of rays
            for (unsigned int p = 0; p < rays.size(); p++) {
                numRays += rays[p].size();
            }

            if (numRays == 4*_numPrtIds) {
                // Send them to the consumer.
                //std::cout << "sending " << numRays << " to the consumer\n";
                _consumer.newRayData(rays);
            } else {
                if (numRays != 0)
                    std::cout << "error: RayCollecter returned " << numRays
                        << " when we were expecting " << 4*_numPrtIds << "\n";
            }
            // we are finished with the current collector crop
            for (unsigned int p = 0; p < rays.size(); p++) {
                for (unsigned int i = 0; i < rays[p].size(); i++) {
                    EldoraDDS::Ray* r = rays[p][i];
                    returnItem(r);
                }
            }
            // return rays that the collector detected as out of sequence
            while (EldoraDDS::Ray* r = collector->unusedRay()) {
                // return the finished rays to DDS
                ///@todo Need to maintain and report an "unwanted" ray count
                returnItem(r);
            }
        } // for collectors
    } // while getNextItem()
}
/////////////////////////////////////////////////////////////////////
std::vector<int> ProductsRayReader::discards()
{
    std::vector<int> n;
    n.resize(2);
    n[0] = _collatorFor.discards();
    n[1] = _collatorAft.discards();
    return n;
}
