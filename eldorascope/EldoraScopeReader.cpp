#include "EldoraScopeReader.h"
#include <QMetaType>
#include <iostream>
using namespace EldoraDDS;

Q_DECLARE_METATYPE(std::vector<double>)

////////////////////////////////////////////////////////
EldoraScopeReader::EldoraScopeReader(
        DDSSubscriber& subscriber,
            std::string topicName,
            int outputRate) :
    TSReader(subscriber, topicName), _readSamples(0), _numBytes(0),
            _outputRate(outputRate), _downCounter(0), _intervalMS(1000),
            _rate(1000000.0), _lastSampleCount(0), _gateMode(ALONG_BEAM),
            _pointsPerGate(0), _gateChoice(0) {

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

            int nci = pItem->nci;/// The current mode of data deliver.

            // the length of each individual time series in the sample. This
            // includs both I and Q.
            int tsLen = (pItem->tsdata.length())/nci;

            switch (_gateMode) {

            case ALONG_BEAM:
                for (int s = 0; s < nci; s++) {
                    _downCounter--;
                    if (_downCounter < 0) {
                        // we multiply the sampe rate by numci, because there are actually
                        // nci time series in each DDS sample
                        _downCounter = (int) ((_rate*nci)/_outputRate);
                       // resize the vectors to carry the beam of IQ data
                        I.resize(tsLen/2);
                        Q.resize(tsLen/2);
                        // extract IQ for the seleced time series within the block of IQ data
                        for (int i = 0; i < tsLen/2; i++) {
                            I[i] = pItem->tsdata[s*tsLen + 2*i ];
                            Q[i] = pItem->tsdata[s*tsLen + 2*i+1];
                        }
                        // send the IQ beam to our client.
emit                                                                         newData(I, Q, 1.0, 100.0);
                    }
                }
                break;
            case ONE_GATE:
                for (int s = 0; s < nci; s++) {
                    _downCounter--;
                    // skip pulse data until downCounter goes negative,
                    // then collect one gate from each pulse until the
                    // specified gates have been saved.
                    if (_downCounter < 0) {
                        I[_pointCounter]
                                = pItem->tsdata[s*tsLen + 2*_gateChoice ];
                        Q[_pointCounter]
                                = pItem->tsdata[s*tsLen + 2*_gateChoice+1];
                        _pointCounter++;
                        if (_pointCounter == _pointsPerGate) {
                            // a set of I/Q points have been collected.
                            // send the IQ beam to our client.
emit                                                         newData(I, Q, 1.0, 100.0);
                            _pointCounter = 0;
                            // downCounter counts down while we are skipping
                            // pulses. when it goes negative, we start collecting 
                            // a timeseries for one gate. Reset the counter to skip the
                            // next dead interval.
                            _downCounter = _rate*nci/_outputRate;
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
    long delta = _readSamples - _lastSampleCount;
    _lastSampleCount = _readSamples;
    _rate = delta/(_intervalMS/1000.0);
    
    std::cout << "rate is " << _rate << "\n";
}

////////////////////////////////////////////////////////////
void EldoraScopeReader::oneGateSlot(
        int gate,
            int n) {
    _gateMode = ONE_GATE;
    _pointsPerGate = n;
    _pointCounter = 0;
    // start timeseries collection immediately
    _downCounter = 0;
    I.resize(n);
    Q.resize(n);
    std::cout << "ONE_GATE mode selected, " << n << " points for gate " << gate
            << "\n";

}

////////////////////////////////////////////////////////////
void EldoraScopeReader::alongBeamSlot() {
    _gateMode = ALONG_BEAM;
    // cause the next pulse to be be sent to the client
    _downCounter = 0;
    std::cout << "ALONG_BEAM mode selected\n";
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
