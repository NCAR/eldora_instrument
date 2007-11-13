#include "EldoraScopeReader.h"

#include "Reader.h"
#include <iostream>
using namespace EldoraDDS;

////////////////////////////////////////////////////////
template<class T> EldoraScopeReader<T>::EldoraScopeReader(
        DDSSubscriber& subscriber,
            std::string topicName,
            SdrScope& scope) :
    T(subscriber, topicName), _scope(scope), _readSamples(0), _numBytes(0) {

}

////////////////////////////////////////////////////////
template<class T> EldoraScopeReader<T>::~EldoraScopeReader() {

}

// Specializations for our data types.

template<> void EldoraScopeReader<PulseReader>::notify() {
    while (Pulse* pItem = EldoraScopeReader<PulseReader>::getNextItem()) {
        _numBytes += pItem->abp.length()*sizeof(pItem->abp[0]);
        EldoraScopeReader<PulseReader>::returnItem(pItem);
    }
}

template<> void EldoraScopeReader<TSReader>::notify() {
    while (TimeSeries* pItem = EldoraScopeReader<TSReader>::getNextItem()) {
        _numBytes += pItem->tsdata.length()*sizeof(pItem->tsdata[0]);
        EldoraScopeReader<TSReader>::returnItem(pItem);
    }
}

////////////////////////////////////////////////////////
template<class T> unsigned long EldoraScopeReader<T>::numBytes() {
    unsigned long n = _numBytes;
    _numBytes = 0;
    return n;
}

// instantiate for the types that we need
template class EldoraScopeReader<PulseReader>;
template class EldoraScopeReader<TSReader>;
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
