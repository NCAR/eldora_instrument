#include "EldoraQtTSSource.h"
#include <QMetaType>
#include <iostream>
using namespace EldoraDDS;

Q_DECLARE_METATYPE(std::vector<double>)
Q_DECLARE_METATYPE(std::vector<int>)

////////////////////////////////////////////////////////
EldoraQtTSSource::EldoraQtTSSource(DDSSubscriber& subscriber,
		std::string topicName, double outputRate) :
	EldoraQtSource(outputRate), TSReader(subscriber, topicName),
			_radarId(EldoraDDS::Forward) {

	// these are required in order to send structured data types
	// via a qt signal
	qRegisterMetaType<std::vector<double> >();
	qRegisterMetaType<std::vector<int> >();

	_radarId = _forwardRadar ? EldoraDDS::Forward : EldoraDDS::Aft;
}

////////////////////////////////////////////////////////
EldoraQtTSSource::~EldoraQtTSSource() {

}

////////////////////////////////////////////////////////
void EldoraQtTSSource::notify() {
	// a DDS data notification has been received.
	while (TimeSeries* pItem = getNextItem()) {

		_readSamples++;
		_numBytes += pItem->tsdata.length()*sizeof(pItem->tsdata[0]);

		// See if this is the channel and radar that we are interested in
		if (pItem->chan == _channel && pItem->radarId == _radarId) {
			// the number of individual timer series in each dds sample

			unsigned int nci = pItem->nci;

			// the length of each individual time series in the sample. This
			// includs both I and Q.
			unsigned int tsLen = (pItem->tsdata.length())/nci;
			// check to see if the number of gates has changed
			if (tsLen/2 != _gates.size()) {
				_gates.resize(tsLen/2);
				for (unsigned int g = 0; g < tsLen/2; g++) {
					_gates[g] = g;
				}
				// since the gate list has changed, default to the first one
				_gate = _gates[0];
				// announce the list of gates
emit                                 				tsGateList(_gates);
			}

			// The data capture strategy will depend on whether we are collecting
			// data along the beam or along the gate.
			switch (_gateMode) {
			case ALONG_BEAM:
				for (unsigned int s = 0; s < nci; s++) {
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
emit                                                     						newData(I, Q, 1.0, 100.0);
						_capture = false;
					}
				}
				break;
			case ONE_GATE:
				for (unsigned int s = 0; s < nci; s++) {
					if (_capture) {
						if (I.size() != _pointsPerGate) {
							I.resize(_pointsPerGate);
							Q.resize(_pointsPerGate);
						}
						I[_pointCounter] = pItem->tsdata[s*tsLen + 2*_gate ];
						Q[_pointCounter] = pItem->tsdata[s*tsLen + 2*_gate+1];
						_pointCounter++;
						if (_pointCounter == _pointsPerGate) {
							// a set of I/Q points have been collected.
							// send the IQ beam to our client.
							_pointCounter = 0;
							// emit the new data signal!
emit                                                          							newData(I, Q, 1.0, 100.0);
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

////////////////////////////////////////////////////////////
void EldoraQtTSSource::oneGateSlot(int channel, bool forwardRadar, int gate,
		int n) {

	std::cout << __FUNCTION__ << "\n";
	
	_gate = gate;
	_channel = channel;
	_pointsPerGate = n;
	_gateMode = ONE_GATE;
	_pointCounter = 0;
	_forwardRadar = forwardRadar;

	_radarId = _forwardRadar ? EldoraDDS::Forward : EldoraDDS::Aft;

	// start timeseries collection immediately
	I.resize(n);
	Q.resize(n);
}

////////////////////////////////////////////////////////////
void EldoraQtTSSource::alongBeamSlot(int channel, bool forwardRadar) {

	std::cout << __FUNCTION__ << "\n";
	
	_gateMode = ALONG_BEAM;
	_channel = channel;
	_forwardRadar = forwardRadar;

	_radarId = _forwardRadar ? EldoraDDS::Forward : EldoraDDS::Aft;
}

