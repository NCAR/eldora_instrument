#include "EldoraQtProductsSource.h"
#include <QMetaType>
#include <iostream>
using namespace EldoraDDS;

Q_DECLARE_METATYPE(std::vector<double>)
Q_DECLARE_METATYPE(std::vector<int>)

////////////////////////////////////////////////////////
EldoraQtProductsSource::EldoraQtProductsSource(
        DDSSubscriber& subscriber,
            std::string topicName,
            double outputRate) :
    EldoraQtSource(outputRate), PulseReader(subscriber, topicName) {

    qRegisterMetaType<std::vector<double> >();
    qRegisterMetaType<std::vector<int> >();

    _radarId = _forwardRadar ? EldoraDDS::Forward : EldoraDDS::Aft;

}

////////////////////////////////////////////////////////
EldoraQtProductsSource::~EldoraQtProductsSource() {

}

////////////////////////////////////////////////////////
void EldoraQtProductsSource::notify() {
    while (Products* pItem = getNextItem()) {

        _readSamples++;
        _numBytes += pItem->dbz.length()*sizeof(pItem->dbz[0]);
        
        if (pItem->chan == _channel && pItem->radarId == _radarId) {

            switch (_gateMode) {

            case ALONG_BEAM:
                if (_capture) {
                    // resize the vectors to carry the beam of product data
                    P.resize(pItem->dbz.length());
                    // copy all P in the beam.
                    for (unsigned int i = 0; i < pItem->dbz.length(); i++) {
                        P[i] = pItem->abp[i ];
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
                    P[_pointCounter] = pItem->dbz[_gate];
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
void EldoraQtProductsSource::oneGateSlot(
        int channel,
            bool forwardRadar,
            int gate,
            int n) {

    EldoraQtSource::oneGateSlot(channel, forwardRadar, gate, n);

    _radarId = _forwardRadar ? EldoraDDS::Forward : EldoraDDS::Aft;
    // start timeseries collection immediately
    P.resize(n);
}

////////////////////////////////////////////////////////////
void EldoraQtProductsSource::alongBeamSlot(
        int channel,
            bool forwardRadar) {
    EldoraQtSource::alongBeamSlot(channel, forwardRadar);

    _radarId = _forwardRadar ? EldoraDDS::Forward : EldoraDDS::Aft;
}
