#include "EldoraQtSource.h"
#include <QMetaType>
#include <iostream>

Q_DECLARE_METATYPE(std::vector<double>)
Q_DECLARE_METATYPE(std::vector<int>)

////////////////////////////////////////////////////////
EldoraQtSource::EldoraQtSource(
		double outputRate) : 
_readSamples(0), 
_numBytes(0),
_gateMode(ALONG_BEAM),
_pointsPerGate(0), 
_pointCounter(0),
_gate(0),
_capture(false), 
_outputRate(outputRate),
_channel(1),
_forwardRadar(true)
{
		    // determine the timer interval. Don't support
		    // output rates greater than 100 Hz.
		    if (_outputRate > 200.0)
		    _outputRate = 200.0;

		    _intervalMS = (int)(1000.0/_outputRate);

		    connect(&_rateTimer, SIGNAL(timeout()), this, SLOT(rateTimeoutSlot()));

		    _rateTimer.setSingleShot(false);
		    _rateTimer.start(_intervalMS);
		    
}

////////////////////////////////////////////////////////
EldoraQtSource::~EldoraQtSource() {

}

////////////////////////////////////////////////////////
void
EldoraQtSource::run() {
   exec();
}

////////////////////////////////////////////////////////
unsigned long EldoraQtSource::numBytes() {
	unsigned long n = _numBytes;
	_numBytes = 0;
	return n;
}

////////////////////////////////////////////////////////////
void EldoraQtSource::rateTimeoutSlot() {
	_capture = true;
}

////////////////////////////////////////////////////////////
void EldoraQtSource::oneGateSlot(int channel,
        bool forwardRadar, int gate, int n) {
	_gate = gate;
	_channel = channel;
	_pointsPerGate = n;
	_gateMode = ONE_GATE;
	_pointCounter = 0;
	_forwardRadar = forwardRadar;
}

////////////////////////////////////////////////////////////
void EldoraQtSource::alongBeamSlot(int channel,
        bool forwardRadar) {
	_gateMode = ALONG_BEAM;
	_channel = channel;
    _forwardRadar = forwardRadar;
}

////////////////////////////////////////////////////////////
void EldoraQtSource::shutdown() {
    
}
