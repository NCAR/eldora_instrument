#include "EldoraQtProductsSource.h"
#include <QMetaType>
#include <iostream>
#include <math.h>

using namespace EldoraDDS;

Q_DECLARE_METATYPE(std::vector<double>)
Q_DECLARE_METATYPE(std::vector<int>)
Q_DECLARE_METATYPE(StrMapDouble)
Q_DECLARE_METATYPE(qlonglong)

////////////////////////////////////////////////////////
EldoraQtProductsSource::EldoraQtProductsSource(
        DDSSubscriber& subscriber, std::string topicName, double outputRate,
        RADAR_CHOICE radarChoices, std::set<PRODUCT_TYPES> productChoices) :
EldoraQtSource(outputRate), ProductsReader(subscriber, topicName),
_outputRate(outputRate), _radarChoices(radarChoices),
_productChoices(productChoices) {

    qRegisterMetaType<std::vector<double> >();
    qRegisterMetaType<std::vector<int> >();
    qRegisterMetaType<StrMapDouble>();
    qRegisterMetaType<qlonglong>();

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
                float gateSpacingMeters = pItem->gateSpacingMeters;
                double dWidth = dwellWidth(pItem);
                double airSpdCorr = airSpeedCorrection(pItem);
                double rollAngle = pItem->hskp.roll;
		        qlonglong timetag = pItem->hskp.timetag;
                
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
                            pP[i] = (((unsigned short)product[i]) - offset)/gain;
                        }
                       // send the Pbeam to our client.
                        emit newPData(P, 
                        		pItem->radarId, 
                        		pItem->rotAngle, 
                                *prodType, 
                                gateSpacingMeters, 
                                dWidth, 
                                airSpdCorr,
                                rollAngle);

			StrMapDouble hskpMap;
                        
			hskpMap["gateSpacingMeters"] = gateSpacingMeters;
			hskpMap["lat"] =pItem->hskp.latitude;
			hskpMap["lon"] =pItem->hskp.longitude;
			hskpMap["rotAngle"] = pItem->rotAngle;
			hskpMap["gateSpacingMeters"] = gateSpacingMeters;
			hskpMap["dwellWidth"] = dWidth;
			hskpMap["elevation"] = pItem->hskp.elevation;
			hskpMap["heading"] = pItem->hskp.heading;
			hskpMap["roll"] = pItem->hskp.roll;
			hskpMap["pitch"] = pItem->hskp.pitch;
			hskpMap["yaw"] = pItem->hskp.yaw;
			hskpMap["groundSpeedEW"] = pItem->hskp.groundSpeedEW;
			hskpMap["groundSpeedNS"] = pItem->hskp.groundSpeedNS;
			hskpMap["airSpdCorr"] = airSpdCorr;
#ifdef DEBUG_HSKP_MAP
                        std::cerr << "EldoraQtProductsSource - emit\n";
                        StrMapDoubleIter si;
                        double value;
                        for (si = hskpMap.begin(); si != hskpMap.end(); ++si){
                            std::string varName = (*si).first;
                            value = (*si).second;
                            std::cerr << varName << " => " << value <<
                                std::endl;
                        }
#endif
                        
			emit newPDataHskpMap( P, 
					  pItem->radarId, 
					  *prodType, 
					  timetag,
					  hskpMap);
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
                        P[_pointCounter] = (((unsigned short)product[_gate])+offset)/gain;
                        _pointCounter++;
                        if (_pointCounter == _pointsPerGate) {
                            // a set of P points have been collected.
                            // send the P time series to our client.
                            _pointCounter = 0;
                            emit newPData(P, 
                            		pItem->radarId, 
                                    pItem->rotAngle, 
                                    *prodType, 
                                    gateSpacingMeters, 
                                    dWidth, 
                                    airSpdCorr,
                                    rollAngle);
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

////////////////////////////////////////////////////////////
double EldoraQtProductsSource::airSpeedCorrection(Products* pItem) {
    
    // compute the along beam ground speed.
	
	EldoraDDS::Housekeeping* pHskp = &pItem->hskp;
    
	// the correction is zero if we are operating in single prt mode
	if (pHskp->prtLong < 0.0)
		return 0.0;
		
    double tiltAngle = pHskp->radarTiltAngle;
    
    // these values come from the INS
    double ns = pHskp->groundSpeedNS;
    double ew = pHskp->groundSpeedEW;
    
    double groundSpeed = sqrt(ns*ns+ew*ew);
    
    // sanity checks
    if (groundSpeed < 70.0 || groundSpeed > 200.0)
    	groundSpeed = 130.0;
    
    double alongBeamGroundSpeed = groundSpeed * sin((M_PI/180.0)* tiltAngle);
    
    if (pItem->radarId == EldoraDDS::Aft)
    	alongBeamGroundSpeed = -alongBeamGroundSpeed;
    
    return alongBeamGroundSpeed;
}

////////////////////////////////////////////////////////////
double EldoraQtProductsSource::nyquistVelocity(Products* pItem) {
	
	EldoraDDS::Housekeeping* pHskp = &pItem->hskp;
    
	double prt;
	if (pHskp->prtLong < 0.0) {
		// single prt
		prt = pHskp->prt;
	} else {
		prt = (pHskp->prt + pHskp->prtLong)/2.0;
	}
	// convert from ms to seconds.
	prt /= 1000.0;

	// just use the first radar frequency for the nyquist? 
	// right now we can't be sure how many frequencies we
	// are using.
	double lambda;
	lambda = pHskp->freqs[0];
	
	double nyquist;
	nyquist = lambda/(4.0*M_PI*prt);
	
	return nyquist;
}

////////////////////////////////////////////////////////////
double EldoraQtProductsSource::dwellWidth(Products* pItem) {
    
    
    double angle = pItem->hskp.rotationVel
    	*pItem->hskp.repeatSeqPerDwell
    	*pItem->hskp.msPerRepeat/1000.0;
    
    // sanity check
    if (angle < 0.05 || angle > 10.0) 
    	angle = 1.0;
    
    return angle;

}

