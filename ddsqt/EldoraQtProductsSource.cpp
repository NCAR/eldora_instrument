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

            // get a pointer to the currently selected product,
            // and the gain and offset.
            float gain;
            float offset;
            short* product;
            selectProduct(pItem, &product, gain, offset);

            switch (_gateMode) {
            case ALONG_BEAM:
                if (_capture) {
                    // resize the vectors to carry the beam of product data
                    P.resize(pItem->dbz.length());
                    // copy all P in the beam.
                    for (unsigned int i = 0; i < pItem->dbz.length(); i++) {
                        P[i] = (product[i] + offset)/gain;
                    }
                    // send the Pbeam to our client.
                    emit newPData(P);
                    clearCapture();
                }
                break;

            case ONE_GATE:
                if (_capture) {
                    if (P.size() != _pointsPerGate) {
                        P.resize(_pointsPerGate);
                    }
                    P[_pointCounter] = (product[_gate]+offset)/gain;
                    _pointCounter++;
                    if (_pointCounter == _pointsPerGate) {
                        // a set of P points have been collected.
                        // send the P time series to our client.
                        _pointCounter = 0;
                        emit newPData(P);
                        clearCapture();
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
void EldoraQtProductsSource::selectProduct(
        Products* pItem, short** p, float& gain, float& offset) {

    switch (_product) {
    case PROD_P1:
        *p = &pItem->p1[0];
        gain = pItem->p1Gain;
        offset = pItem->p1Offset;
        break;
    case PROD_P2:
        *p = &pItem->p2[0];
        gain = pItem->p3Gain;
        offset = pItem->p3Offset;
        break;
    case PROD_P3:
        *p = &pItem->p3[0];
        gain = pItem->p3Gain;
        offset = pItem->p3Offset;
        break;
    case PROD_P4:
        *p = &pItem->p4[0];
        gain = pItem->p4Gain;
        offset = pItem->p4Offset;
        break;
    case PROD_VR:
        *p = &pItem->vr[0];
        gain = pItem->vrGain;
        offset = pItem->vrOffset;
        break;
    case PROD_VS:
        *p = &pItem->vs[0];
        gain = pItem->vsGain;
        offset = pItem->vsOffset;
        break;
    case PROD_VL:
        *p = &pItem->vl[0];
        gain = pItem->vlGain;
        offset = pItem->vlOffset;
        break;
    case PROD_DM:
        *p = &pItem->dm[0];
        gain = pItem->dmGain;
        offset = pItem->dmOffset;
        break;
    case PROD_DBZ:
        *p = &pItem->dbz[0];
        gain = pItem->dbzGain;
        offset = pItem->dbzOffset;
        break;
    case PROD_SW:
        *p = &pItem->sw[0];
        gain = pItem->swGain;
        offset = pItem->swOffset;
        break;
    case PROD_NCP:
        *p = &pItem->ncp[0];
        gain = pItem->ncpGain;
        offset = pItem->ncpOffset;
        break;
    default:
        // what the heck, just use dbz
        *p = &pItem->dbz[0];
        gain = pItem->dbzGain;
        offset = pItem->dbzOffset;
        break;
    }
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
