#include "EldoraScopeReader.h"
#include <QMetaType>
#include <iostream>
using namespace EldoraDDS;

Q_DECLARE_METATYPE(std::vector<double>)

////////////////////////////////////////////////////////
EldoraScopeReader::EldoraScopeReader(
        DDSSubscriber& subscriber,
        std::string topicName) :
TSReader(subscriber, topicName),
_readSamples(0),
_numBytes(0),
_decimation(100) {

    qRegisterMetaType<std::vector<double> >();

}

////////////////////////////////////////////////////////
EldoraScopeReader::~EldoraScopeReader() {

}

void EldoraScopeReader::notify() {
    while (TimeSeries* pItem = getNextItem()) {
        _readSamples++;
        _numBytes += pItem->tsdata.length()*sizeof(pItem->tsdata[0]);
        if (!(_readSamples % _decimation)) {
            std::vector<double> I;
            std::vector<double> Q;
            I.resize(pItem->tsdata.length()/2);
            Q.resize(pItem->tsdata.length()/2);
            for (int i = 0; i < pItem->tsdata.length()/2; i++) {
                I[i] = pItem->tsdata[2*i];
                Q[i] = pItem->tsdata[2*i+1];
            }
            emit newData(I, Q, 1.0, 100.0);
        }
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
