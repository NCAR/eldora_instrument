#include "ProdABPreader.h"
#include <iostream>
/////////////////////////////////////////////////////////////////////
ProdABPreader::ProdABPreader(
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
ProdABPreader::~ProdABPreader() {

}

/////////////////////////////////////////////////////////////////////
void ProdABPreader::notify() {
    while (Pulse* pPulse = getNextItem()) {

        // give the pulse to the fore or aft collector.
        // One of them will accept it. Note that 
        // when the pulse timestamp changes, the 
        _collectorFore.addPulse(pPulse);
        _collectorAft.addPulse(pPulse);

        // process forward and aft pulses
        for (unsigned int c = 0; c < _collectors.size(); c++) {
            ABPCollector* collector = _collectors[c];

            // Do we have four pulses? 
            if (collector->size() == 4) {
                // We have four matched pulses. Sum them over the
                // frequencies, saving in the first pulse.
                for (unsigned int f = 1; f < 4; f++) {
                    for (unsigned int j = 0; j
                            < (*collector)[0]->abp.length(); j++) {
                        (*collector)[0]->abp[j] += (*collector)[f]->abp[j];
                    }
                }
                // Send the summed ABP to the consumer.
                _consumer.newABPdata((*collector)[0]);
                // we are finished with the current collector crop
                collector->flush();
            }
            // return pulses that are finished
            while (EldoraDDS::Pulse* p = collector->finishedPulse()) {
                // return the finished pulses to DDS
                returnItem(p);
            }
        } // for collectors
    } // while getNextItem()
}
/////////////////////////////////////////////////////////////////////
std::vector<int> ProdABPreader::discards() {
    std::vector<int> n;
    n.resize(2);
    n[0] = _collectorFore.discards();
    n[1] = _collectorAft.discards();
    return n;
}
