#include "ProdABPreader.h"
#include <iostream>
/////////////////////////////////////////////////////////////////////
ProdABPreader::ProdABPreader(
        DDSSubscriber& subscriber,
            std::string abpTopic,
            EldoraProducts& consumer) :
    PulseReader(subscriber, abpTopic), _consumer(consumer) {

}

/////////////////////////////////////////////////////////////////////
ProdABPreader::~ProdABPreader() {

}

/////////////////////////////////////////////////////////////////////
void ProdABPreader::notify() {
    while (Pulse* pPulse = getNextItem()) {
        // send the pulse to the consumer
        _consumer.newABPdata(pPulse);

        // return the pulse to DDS
        returnItem(pPulse);
    }
}
