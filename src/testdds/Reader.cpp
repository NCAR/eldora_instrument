#include "Reader.h"
#include <iostream>
using namespace EldoraDDS;

////////////////////////////////////////////////////////
template<class T> TestReader<T>::TestReader(DDSSubscriber& subscriber,
		std::string topicName) :
	T(subscriber, topicName), _readSamples(0), _numBytes(0) {

}

////////////////////////////////////////////////////////
template<class T> TestReader<T>::~TestReader() {

}

// Specializations for our data types.

template<> void TestReader<RayReader>::notify() {
	while (Ray* pItem = TestReader<RayReader>::getNextItem()) {
		_numBytes += pItem->abp.length()*sizeof(pItem->abp[0]);
		TestReader<RayReader>::returnItem(pItem);
	}
}

template<> void TestReader<TSReader>::notify() {
	while (TimeSeries* pItem = TestReader<TSReader>::getNextItem()) {
		_numBytes += pItem->tsdata.length()*sizeof(pItem->tsdata[0]);
		TestReader<TSReader>::returnItem(pItem);
	}
}

////////////////////////////////////////////////////////
template<class T> unsigned long TestReader<T>::numBytes() {
	unsigned long n = _numBytes;
	_numBytes = 0;
	return n;
}

// instantiate for the types that we need
template class TestReader<RayReader>;
template class TestReader<TSReader>;
