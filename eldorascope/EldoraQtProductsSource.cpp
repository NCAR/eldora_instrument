#include "EldoraQtProductsSource.h"
#include <QMetaType>
#include <iostream>
using namespace EldoraDDS;

Q_DECLARE_METATYPE(std::vector<double>)
Q_DECLARE_METATYPE(std::vector<int>)

////////////////////////////////////////////////////////
EldoraQtProductsSource::EldoraQtProductsSource(
        DDSSubscriber& subscriber, std::string topicName, double outputRate) :
    EldoraQtSource(outputRate), ProductsReader(subscriber, topicName),
            _radarId(EldoraDDS::Forward), _product(PROD_DBZ) {

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
        
        if (pItem->radarId == _radarId) {

            // get a pointer to the currently selected product.
            float* product = selectProduct(pItem);

            switch (_gateMode) {
            case ALONG_BEAM:
                if (_capture) {
                    // resize the vectors to carry the beam of product data
                    P.resize(pItem->dbz.length());
                    // copy all P in the beam.
                    for (unsigned int i = 0; i < pItem->dbz.length(); i++) {
                        P[i] = product[i ];
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
                    P[_pointCounter] = product[_gate];
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
float* EldoraQtProductsSource::selectProduct(
        Products* pItem) {

    float* p;
    switch (_product) {
    case PROD_P1:
        p = &pItem->p1[0];
        break;
    case PROD_P2:
        p = &pItem->p2[0];
        break;
    case PROD_P3:
        p = &pItem->p3[0];
        break;
    case PROD_P4:
        p = &pItem->p4[0];
        break;
    case PROD_DBM:
        p = &pItem->dbm[0];
        break;
    case PROD_DBZ:
        p = &pItem->dbz[0];
        break;
    case PROD_VEL:
        p = &pItem->vel[0];
        break;
    case PROD_SNR:
        p = &pItem->snr[0];
        break;
    case PROD_WIDTH:
        p = &pItem->width[0];
        break;
    case PROD_NCP:
        p = &pItem->ncp[0];
        break;
    default:
        // what the heck, just use dbz
        p = &pItem->dbz[0];
        break;
    }
    
    return p;
}

////////////////////////////////////////////////////////////
void EldoraQtProductsSource::oneGateSlot(
        PRODUCT_TYPES product, bool forwardRadar, int gate, int n) {

   _gate = gate;
    _pointsPerGate = n;
    _gateMode = ONE_GATE;
    _pointCounter = 0;
    _forwardRadar = forwardRadar;
    _product = product;

    _radarId = _forwardRadar ? EldoraDDS::Forward : EldoraDDS::Aft;
    // start timeseries collection immediately
    P.resize(n);
}

////////////////////////////////////////////////////////////
void EldoraQtProductsSource::alongBeamSlot(
        PRODUCT_TYPES product, bool forwardRadar) {

    _gateMode = ALONG_BEAM;
    _product = product;
    _forwardRadar = forwardRadar;

    _radarId = _forwardRadar ? EldoraDDS::Forward : EldoraDDS::Aft;
}
