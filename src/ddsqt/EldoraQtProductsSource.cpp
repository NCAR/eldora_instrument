#include "EldoraQtProductsSource.h"
#include <QMetaType>
#include <iostream>
using namespace EldoraDDS;

Q_DECLARE_METATYPE(std::vector<double>)
Q_DECLARE_METATYPE(std::vector<int>)

////////////////////////////////////////////////////////
EldoraQtProductsSource::EldoraQtProductsSource(
        DDSSubscriber& subscriber, std::string topicName, double outputRate,
        RADAR_CHOICE radarChoices, std::set<PRODUCT_TYPES> productChoices) :
EldoraQtSource(outputRate), ProductsReader(subscriber, topicName),
_outputRate(outputRate), _radarChoices(radarChoices),
_productChoices(productChoices) {

    qRegisterMetaType<std::vector<double> >();
    qRegisterMetaType<std::vector<int> >();

}

////////////////////////////////////////////////////////
EldoraQtProductsSource::~EldoraQtProductsSource() {

}

////////////////////////////////////////////////////////
void EldoraQtProductsSource::notify() {
    while (Products* pItem = getNextItem()) {

        _readSamples++;
        _numBytes += pItem->dbz.length()*sizeof(pItem->dbz[0]);

        RADAR_CHOICE thisRadar;
        switch (pItem->radarId) {
        case EldoraDDS::Forward:
            thisRadar = RADAR_FOR;
            break;
        case EldoraDDS::Aft:
            thisRadar = RADAR_AFT;
            break;
        default:
            thisRadar = RADAR_FOR;
            break;
        }
        
//        std::cout << "_radarChoices:" << _radarChoices <<
//        "  thisRadar:" << thisRadar << "\n";
        
        unsigned int productSize = pItem->dbz.length();
        if (_radarChoices == RADAR_BOTH || _radarChoices == thisRadar) {

            for (std::set<PRODUCT_TYPES>::iterator prodType = _productChoices.begin(); 
                prodType != _productChoices.end(); prodType++) {
                // get a pointer to the desired product,
                // and the gain and offset.
                float gain;
                float offset;
                short* product;
                
                // get the scaling and offset values for this product type
                selectProduct(*prodType, pItem, &product, gain, offset);

                switch (_gateMode) {
                case ALONG_BEAM:
                    if (_capture) {
                        if (productSize != P.size()) {
                            // resize the vectors to carry the beam of product data
                            P.resize(pItem->dbz.length());
                        }
                        // copy all P in the beam.
                        double* pP = &P[0];
                        for (unsigned int i = 0; i < productSize; i++) {
                            pP[i] = (product[i] - offset)/gain;
                        }
                       // send the Pbeam to our client.
                        emit newPData(P, pItem->radarId, pItem->rotAngle, 
                                *prodType);
                        clearCapture();
                    }
                    break;

                case ONE_GATE:
                    // note that setting the _bothRadars flag along
                    // with the ONE_GATE option will cause the data
                    // for fore and aft to be scrambled together.
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
                            emit newPData(P, pItem->radarId, 
                                    pItem->rotAngle, *prodType);
                            clearCapture();
                        }
                    }
                    break;
                }
            }
        }

        // return the dds sample
        returnItem(pItem);
    }
}

////////////////////////////////////////////////////////////
void EldoraQtProductsSource::selectProduct(
        PRODUCT_TYPES prodType, Products* pItem, short** p, float& gain,
        float& offset) {

	switch (prodType) {
    case PROD_P1:
        *p = &pItem->p1[0];
        gain = pItem->p1Scale;
        offset = pItem->p1Offset;
        break;
    case PROD_P2:
        *p = &pItem->p2[0];
        gain = pItem->p3Scale;
        offset = pItem->p3Offset;
        break;
    case PROD_P3:
        *p = &pItem->p3[0];
        gain = pItem->p3Scale;
        offset = pItem->p3Offset;
        break;
    case PROD_P4:
        *p = &pItem->p4[0];
        gain = pItem->p4Scale;
        offset = pItem->p4Offset;
        break;
    case PROD_VR:
        *p = &pItem->vr[0];
        gain = pItem->vrScale;
        offset = pItem->vrOffset;
        break;
    case PROD_VS:
        *p = &pItem->vs[0];
        gain = pItem->vsScale;
        offset = pItem->vsOffset;
        break;
    case PROD_VL:
        *p = &pItem->vl[0];
        gain = pItem->vlScale;
        offset = pItem->vlOffset;
        break;
    case PROD_DM:
        *p = &pItem->dm[0];
        gain = pItem->dmScale;
        offset = pItem->dmOffset;
        break;
    case PROD_DBZ:
        *p = &pItem->dbz[0];
        gain = pItem->dbzScale;
        offset = pItem->dbzOffset;
        break;
    case PROD_SW:
        *p = &pItem->sw[0];
        gain = pItem->swScale;
        offset = pItem->swOffset;
        break;
    case PROD_NCP:
        *p = &pItem->ncp[0];
        gain = pItem->ncpScale;
        offset = pItem->ncpOffset;
        break;
    default:
        // what the heck, just use dbz
        *p = &pItem->dbz[0];
        gain = pItem->dbzScale;
        offset = pItem->dbzOffset;
        break;
    }
}

////////////////////////////////////////////////////////////
void EldoraQtProductsSource::oneGateSlot(
        PRODUCT_TYPES prodType, bool forwardRadar, int gate, int n) {

    _gate = gate;
    _pointsPerGate = n;
    _gateMode = ONE_GATE;
    _pointCounter = 0;
    _forwardRadar = forwardRadar;
    
    // if we are not already looking at multiple products, then
    // change the one that we are looking at
    if (_productChoices.size() <= 1) {
    _productChoices.clear();
    _productChoices.insert(prodType);
    }

    _radarChoices = _forwardRadar ? RADAR_FOR : RADAR_AFT;
    
    // start timeseries collection immediately
    P.resize(n);
}

////////////////////////////////////////////////////////////
void EldoraQtProductsSource::alongBeamSlot(
        PRODUCT_TYPES prodType, bool forwardRadar) {

    // set the gate mode
    _gateMode = ALONG_BEAM;
    
    // save the radar
    _forwardRadar = forwardRadar;
    
    // if we are not already looking at multiple products, then
    // change the one that we are looking at
    if (_productChoices.size() <= 1) {
    _productChoices.clear();
    _productChoices.insert(prodType);
    }
    
    _radarChoices = _forwardRadar ? RADAR_FOR : RADAR_AFT;
}
