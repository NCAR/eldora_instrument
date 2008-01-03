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
    if (_outputRate > 200.0)
        _outputRate = 200.0;

    _intervalMS = (int)(1000.0/_outputRate);

    connect(&_rateTimer, SIGNAL(timeout()), this, SLOT(rateTimeoutSlot()));

    // start the capture timer. Note that the _capture flag will
    // not be set until _run is set.
    _rateTimer.setSingleShot(false);
    _rateTimer.start(_intervalMS);


}

////////////////////////////////////////////////////////
EldoraQtSource::~EldoraQtSource() {

}

////////////////////////////////////////////////////////
void EldoraQtSource::run() {

    // Indicate that the event loop is running.
    _run = true;
    
    std::cout << __FILE__ << ":" << __FUNCTION__ << "\n";
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
void EldoraQtSource::shutdown() {

}
