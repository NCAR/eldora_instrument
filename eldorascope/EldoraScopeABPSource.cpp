#include "EldoraScopeABPSource.h"
#include <QMetaType>
#include <iostream>
using namespace EldoraDDS;

Q_DECLARE_METATYPE(std::vector<double>)
Q_DECLARE_METATYPE(std::vector<int>)

////////////////////////////////////////////////////////
EldoraScopeABPSource::EldoraScopeABPSource(
        DDSSubscriber& subscriber,
            std::string topicName,
            double outputRate) :
    EldoraScopeSource(outputRate), PulseReader(subscriber, topicName) {

    qRegisterMetaType<std::vector<double> >();
    qRegisterMetaType<std::vector<int> >();

    _radarId = _forwardRadar ? EldoraDDS::Forward : EldoraDDS::Aft;

}

////////////////////////////////////////////////////////
EldoraScopeABPSource::~EldoraScopeABPSource() {

}

////////////////////////////////////////////////////////
void EldoraScopeABPSource::notify() {
    while (Pulse* pItem = getNextItem()) {

        _readSamples++;
        _numBytes += pItem->abp.length()*sizeof(pItem->abp[0]);
        if (pItem->chan == _channel && pItem->radarId == _radarId) {

            switch (_gateMode) {

            case ALONG_BEAM:
                if (_capture) {
                    // resize the vectors to carry the beam of IQ data
                    P.resize(pItem->abp.length()/3);
                    // copy all P in the beam.
                    for (unsigned int i = 2; i < pItem->abp.length(); i += 3) {
                        P[i/3] = pItem->abp[i ];
                    }
                    // send the Pbeam to our client.
                    emit newPData(P);
                    _capture = false;
                }
                break;

            case ONE_GATE:
                if (_capture) {
                    if (P.size() != _pointsPerGate) {
                        P.resize(_pointsPerGate);
                    }
                    P[_pointCounter] = pItem->abp[3*_gate+2];
                    _pointCounter++;
                    if (_pointCounter == _pointsPerGate) {
                        // a set of P points have been collected.
                        // send the P time series to our client.
                        _pointCounter = 0;
                        emit newPData(P);
                        _capture = false;
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
void EldoraScopeABPSource::oneGateSlot(
        int channel,
            bool forwardRadar,
            int gate,
            int n) {

    EldoraScopeSource::oneGateSlot(channel, forwardRadar, gate, n);

    _radarId = _forwardRadar ? EldoraDDS::Forward : EldoraDDS::Aft;
    // start timeseries collection immediately
    P.resize(n);
}

////////////////////////////////////////////////////////////
void EldoraScopeABPSource::alongBeamSlot(
        int channel,
            bool forwardRadar) {
    EldoraScopeSource::alongBeamSlot(channel, forwardRadar);

    _radarId = _forwardRadar ? EldoraDDS::Forward : EldoraDDS::Aft;
}

