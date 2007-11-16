#include "EldoraScopeReader.h"
#include <QMetaType>
#include <iostream>
using namespace EldoraDDS;

Q_DECLARE_METATYPE(std::vector<double>)

////////////////////////////////////////////////////////
EldoraScopeReader::EldoraScopeReader(DDSSubscriber& subscriber,
		std::string topicName, int outputRate) :
	TSReader(subscriber, topicName), _readSamples(0), _numBytes(0),
			_outputRate(outputRate), _downCounter(0), _intervalMS(1000),
			_rate(0.0), _lastSampleCount(0) {

	qRegisterMetaType<std::vector<double> >();

	connect(&_rateTimer, SIGNAL(timeout()), this, SLOT(rateTimeoutSlot()));

	_rateTimer.setSingleShot(false);
	_rateTimer.start(_intervalMS);
}

////////////////////////////////////////////////////////
EldoraScopeReader::~EldoraScopeReader() {

}

////////////////////////////////////////////////////////
void EldoraScopeReader::notify() {
	while (TimeSeries* pItem = getNextItem()) {

		_readSamples++;
		_numBytes += pItem->tsdata.length()*sizeof(pItem->tsdata[0]);

		if (pItem->chan == 0) {
			// the number of individual timer series in each dds sample

			int nci = pItem->nci;
			// the length of each individual time series in the sample. This
			// includs both I and Q.
			int tsLen = (pItem->tsdata.length())/nci;

			for (int s = 0; s < nci; s++) {
				_downCounter--;
				if (_downCounter < 0) {
					// we multiply the sampe rate by numci, because there are actually
					// nci time series in each DDS sample
					_downCounter = (int) ((_rate*nci)/_outputRate);

					// create the vectors to carry the IQ data
					std::vector<double> I;
					std::vector<double> Q;
					I.resize(tsLen/2);
					Q.resize(tsLen/2);

					// extract IQ for the seleced time series within the block of IQ data
					for (int i = 0; i < tsLen/2; i++) {
						I[i] = pItem->tsdata[s*tsLen + 2*i ];
						Q[i] = pItem->tsdata[s*tsLen + 2*i+1];
					}

					// send the IQ vectors to our client.
                    emit newData(I, Q, 1.0, 100.0);
				}
			}
		}

		// return the dds sample
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
void EldoraScopeReader::rateTimeoutSlot() {
	long delta = _readSamples - _lastSampleCount;
	_lastSampleCount = _readSamples;
	_rate = delta/(_intervalMS/1000.0);
}
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
