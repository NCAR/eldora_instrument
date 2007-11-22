#include "EldoraScopeReader.h"
#include <QMetaType>
#include <iostream>
using namespace EldoraDDS;

Q_DECLARE_METATYPE(std::vector<double>)
Q_DECLARE_METATYPE(std::vector<int>)

////////////////////////////////////////////////////////
EldoraScopeReader::EldoraScopeReader(
		DDSSubscriber& subscriber,
		std::string topicName,
		double outputRate) :
TSReader(subscriber, topicName), 
_readSamples(0), 
_numBytes(0),
_gateMode(ALONG_BEAM),
_pointsPerGate(0), _gate(0),
_capture(false), 
_outputRate(outputRate),
_channel(1)
{

	// determine the timer interval. Don't support
	// output rates greater than 100 Hz.
	if (_outputRate > 200.0)
	_outputRate = 200.0;

	_intervalMS = (int)(1000.0/_outputRate);
	qRegisterMetaType<std::vector<double> >();
	qRegisterMetaType<std::vector<int> >();

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

		if (pItem->chan == _channel) {
			// the number of individual timer series in each dds sample

			int nci = pItem->nci;/// The current mode of data deliver.

			// the length of each individual time series in the sample. This
			// includs both I and Q.
			unsigned int tsLen = (pItem->tsdata.length())/nci;

			// check to see if the number of gates has changed
			if (tsLen/2 != _gates.size()) {
				_gates.resize(tsLen/2);
				for (unsigned int g = 0; g < tsLen/2; g++) {
					_gates[g] = g;
				}
				// announce the list of gates
emit 				gateList(_gates);
			}

			switch (_gateMode) {

			case ALONG_BEAM:
				for (int s = 0; s < nci; s++) {
					if (_capture) {
						// resize the vectors to carry the beam of IQ data
						I.resize(tsLen/2);
						Q.resize(tsLen/2);
						// extract IQ for the seleced time series within the block of IQ data
						for (unsigned int i = 0; i < tsLen/2; i++) {
							I[i] = pItem->tsdata[s*tsLen + 2*i ];
							Q[i] = pItem->tsdata[s*tsLen + 2*i+1];
						}
						// send the IQ beam to our client.
emit 						newData(I, Q, 1.0, 100.0);
						_capture = false;
					}
				}
				break;
			case ONE_GATE:
				for (int s = 0; s < nci; s++) {
					if (_capture) {
						I[_pointCounter] = pItem->tsdata[s*tsLen + 2*_gate ];
						Q[_pointCounter] = pItem->tsdata[s*tsLen + 2*_gate+1];
						_pointCounter++;
						if (_pointCounter == _pointsPerGate) {
							// a set of I/Q points have been collected.
							// send the IQ beam to our client.
							_pointCounter = 0;
emit 							newData(I, Q, 1.0, 100.0);
							_capture = false;
						}
					}
				}
				break;
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
	_capture = true;
}

////////////////////////////////////////////////////////////
void EldoraScopeReader::oneGateSlot(int channel, int gate, int n) {
	_gate = gate;
	_channel = channel;
	_pointsPerGate = n;
	_gateMode = ONE_GATE;
	_pointCounter = 0;
	// start timeseries collection immediately
	I.resize(n);
	Q.resize(n);
}

////////////////////////////////////////////////////////////
void EldoraScopeReader::alongBeamSlot(int channel) {
	_gateMode = ALONG_BEAM;
	_channel = channel;
}

////////////////////////////////////////////////////////////
