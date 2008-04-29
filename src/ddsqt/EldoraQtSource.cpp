#include "EldoraQtSource.h"
#include <QMetaType>
#include <iostream>

Q_DECLARE_METATYPE(std::vector<double>)
Q_DECLARE_METATYPE(std::vector<int>)

////////////////////////////////////////////////////////
EldoraQtSource::EldoraQtSource(
        double outputRate) :
    _readSamples(0), _numBytes(0), _gateMode(ALONG_BEAM), _pointsPerGate(0),
            _pointCounter(0), _gate(0), _capture(false),
            _outputRate(outputRate), _forwardRadar(true), _run(false) {
    // determine the timer interval. Don't support
    // output rates greater than 100 Hz.
    if (_outputRate > 100.0)
        _outputRate = 100.0;

    if (_outputRate > 0.0) 
        _intervalMS = (int)(1000.0/_outputRate);
    else
        _intervalMS = 0;        

    connect(&_rateTimer, SIGNAL(timeout()), this, SLOT(rateTimeoutSlot()));

    // start the capture timer. Note that the _capture flag will
    // not be set until _run is set.
    _rateTimer.setSingleShot(false);
    if (_intervalMS > 0 ) {
        _rateTimer.start(_intervalMS);
    } else {
        // in the case of continuos data, we will poll the _run flag
        // every second, to initiate the capture once.
        _rateTimer.start(1000);        
    }


}

////////////////////////////////////////////////////////
EldoraQtSource::~EldoraQtSource() {

}

////////////////////////////////////////////////////////
void EldoraQtSource::run() {

    // Indicate that the event loop is running.
    _run = true;

    // start the event loop
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
    // Set the capture flag, but only after the _run
    // flag has been set.
    if (_run)
        _capture = true;
}

////////////////////////////////////////////////////////////
void EldoraQtSource::clearCapture() {
    // if we are rate limiting, clear the capture flag.
    if (_intervalMS > 0) {
        _capture = false;
        return;
    }
    
    // we are in continuous mode, so clear it only if _run is cleared.
    _capture = _run;
}

////////////////////////////////////////////////////////////
void EldoraQtSource::shutdown() {

}
