#include "EldoraScopeReader.h"
#include <QMetaType>
#include <iostream>
using namespace EldoraDDS;

Q_DECLARE_METATYPE(std::vector<double>)

////////////////////////////////////////////////////////
EldoraScopeReader::EldoraScopeReader(
        DDSSubscriber& subscriber,
            std::string topicName) :
    PulseReader(subscriber, topicName), _readSamples(0), _numBytes(0) {
            
            qRegisterMetaType<std::vector<double> >();

}

////////////////////////////////////////////////////////
EldoraScopeReader::~EldoraScopeReader() {

}

void EldoraScopeReader::notify() {
    while (Pulse* pItem = getNextItem()) {
        std::vector<double> I;
        std::vector<double> Q;
        I.resize(pItem->abp.length()/3);
        Q.resize(pItem->abp.length()/3);
        for (int i = 0; i < pItem->abp.length()/3; i++) {
            I[i] = pItem->abp[3*i];
            Q[i] = pItem->abp[3*i+1];
        }
emit         newData(I, Q, 1.0, 100.0);
        _numBytes += pItem->abp.length()*sizeof(pItem->abp[0]);
        returnItem(pItem);
    }
}

////////////////////////////////////////////////////////
unsigned long EldoraScopeReader::numBytes() {
    unsigned long n = _numBytes;
    _numBytes = 0;
    return n;
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
