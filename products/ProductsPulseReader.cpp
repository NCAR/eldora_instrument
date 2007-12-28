#include "ProductsPulseReader.h"
#include <iostream>
/////////////////////////////////////////////////////////////////////
ProductsPulseReader::ProductsPulseReader(
        DDSSubscriber& subscriber,
            std::string abpTopic,
            EldoraProducts& consumer) :
    PulseReader(subscriber, abpTopic), _consumer(consumer),
            _collectorFore(EldoraDDS::Forward), _collectorAft(EldoraDDS::Aft) {
    // save references to both collectors
    _collectors.push_back(&_collectorFore);
    _collectors.push_back(&_collectorAft);
}

/////////////////////////////////////////////////////////////////////
ProductsPulseReader::~ProductsPulseReader() {
    std::cout << __FUNCTION__ << " destructor\n";
}

/////////////////////////////////////////////////////////////////////
void ProductsPulseReader::notify() {
    while (Pulse* pPulse = getNextItem()) {

        // give the pulse to the fore or aft collector.
        // One of them should accept it. If not,
    	// we have problems (probably a corrupted
    	// radarId.
        if (!_collectorFore.addPulse(pPulse)) {
        	if (!_collectorAft.addPulse(pPulse)) {
        		// neither collector wanted the pulse!
        		returnItem(pPulse);
        	}
        }

        // now process any forward and aft quad pulses that are available 
        for (unsigned int c = 0; c < _collectors.size(); c++) {
            PulseCollator* collector = _collectors[c];

            // Do we have four pulses? 
            std::vector<EldoraDDS::Pulse*> pulses = collector->pulsesReady();
            if (pulses.size() == 4) {
                // We have four matched pulses. Sum them over the
                // frequencies, saving in the first pulse.
                for (unsigned int f = 1; f < 4; f++) {
                    for (unsigned int j = 0; j
                            < pulses[0]->abp.length(); j++) {
                        pulses[0]->abp[j] += pulses[f]->abp[j];
                    }
                }
                // Send the summed ABP to the consumer.
                _consumer.newABPdata(pulses[0]);
                // we are finished with the current collector crop
                for (unsigned int i = 0; i < 4; i++)
                	returnItem(pulses[i]);
            }
            // return pulses that the collected detected as out of sequence
            while (EldoraDDS::Pulse* p = collector->finishedPulse()) {
                // return the finished pulses to DDS
                returnItem(p);
            }
        } // for collectors
    } // while getNextItem()
}
/////////////////////////////////////////////////////////////////////
std::vector<int> ProductsPulseReader::discards() {
    std::vector<int> n;
    n.resize(2);
    n[0] = _collectorFore.discards();
    n[1] = _collectorAft.discards();
    return n;
}
