#include "TestReader.h"
#include <iostream>
using namespace EldoraDDS;

////////////////////////////////////////////////////////
TestReader::TestReader(DDSSubscriber& subscriber, std::string topicName) :
	DDSReader<Pulse, PulseTypeSupportImpl, PulseTypeSupport_var,
			PulseDataReader, PulseDataReader_var>(subscriber, topicName),
			_readSamples(0), _numBytes(0) {

}

////////////////////////////////////////////////////////

TestReader::~TestReader() {

}

////////////////////////////////////////////////////////

void TestReader::notify() {
	while (Pulse* pItem = getNextItem()) {
		_numBytes += pItem->abp.length()*sizeof(pItem->abp[0]);
		returnItem(pItem);
	}
}

////////////////////////////////////////////////////////

unsigned long TestReader::numBytes() {
	unsigned long n = _numBytes;
	_numBytes = 0;
	return n;
}
