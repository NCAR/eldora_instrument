#include "EldoraProducts.h"
#include <iostream>
#include <math.h>

/// Macro used to apply scale and bias to convert to 16 bit 
/// integer int for on-the-wire transfer
#define TOSHORT(data, scale, bias) ((unsigned short)(data*scale+bias))

/// A squaring function
#define square(a) (a*a)

////////////////////////////////////////////////////
EldoraProducts::EldoraProducts(DDSPublisher& publisher,
                               std::string productsTopic,
                               int nChan,
                               bool dualPrt,
                               bool reverseVelocity) :
    _rays(0), _publisher(publisher), _productsTopic(productsTopic),
            _productsWriter(publisher, productsTopic), _droppedRays(0),
            _nChan(nChan),
            _dualPrt(dualPrt)
{
    if (reverseVelocity)
        _velSign = -1;
    else
        _velSign = 1;
    
    _first[0] = true;
    _first[1] = true;
    _termsInitialized[0] = false;
    _termsInitialized[1] = false;
}

////////////////////////////////////////////////////
EldoraProducts::~EldoraProducts()
{

}

////////////////////////////////////////////////////
void EldoraProducts::newRayData(std::vector<std::vector<EldoraDDS::Ray*> >& rays)
{

    // We have four matched rays for each prt 

	_id = rays[0][0]->radarId;
    _rays++;

    // make sure that we got the right number of prts
    if ((_dualPrt && rays.size() != 2) || (!_dualPrt && rays.size() != 1)) {
        std::cout << "prt mode does not match the number of prt ids received\n";
        _droppedRays++;
        return;
    }

    _gates = rays[0][0]->abp.length()/3;
    initTerms(rays);

    if (_first[_id]) {
    	_first[_id] = false;
    	EldoraDDS::Housekeeping* hskp = &rays[0][0]->hskp;
        std::cout << "Products has received first ray for radarId " 
        << _id
        << " channels:"
        << _nChan
        << " prt:"
        << hskp->prt 
        << " prtLong:"
        << hskp->prtLong 
        << "  gate spacing:" 
        << hskp->cellWidth[0] 
        << "\n";
        std::cout
        << "radarConstant:" << hskp->radarConstant
        << "  freqs[0]:" << hskp->freqs[0]
        << "  r[13]:" << _terms[_id].r[13]
        << "  r[0]:" << _terms[_id].r[0]
        << "  firstGate:" << hskp->firstGateDistance
        << "\n";  
    }
	
	// get an empty products container from DDS
    Products* products = _productsWriter.getEmptyItem();
	
    if (products) {

        // The abp rays are three times as long as the
        // product rays, since it contains A, B and P. 
        int gates = rays[0][0]->abp.length()/3;

        // initiallize the fixed fields in Products, and resize the vectors.
        initProducts(products, rays[0][0]->hskp, gates);

        // This is where the real work gets done.
        // Compute the various terms found in the Loew paper,
		// following the order given in the paper.
		// The _terms structure gets filled in with 
		// results as the steps are completed.
        
		// precompute some common sums.
		computeSums(rays);
        
		// power
		powerRaw(rays);
        
		// power at antenna
		powerAntenna(rays);
        
		// total power
		totalPower(rays);
        
		// signal power
		signalPower(rays);
        
		// total signal power
		totalSignalPower(rays);
        
		// dbz
		reflectivity(rays);
        
		// velocity terms.
		velocity(rays);
        
		// spectrum width
		spectrumWidth(rays);
        
		// normalized coherent power
		ncp(rays);

        // Comutations ar edone. Fill in the product ray
		
        // First transfer ray metadata
        products->radarId = rays[0][0]->radarId;
        products->timestamp = rays[0][0]->hskp.rayNum;
        products->rotAngle = rays[0][0]->hskp.radarRotAngle;
        products->gateSpacingMeters = rays[0][0]->hskp.cellWidth[0];

        // Transfer the final results from _terms to products.
        for (int g = 0; g < gates; g++) {
            if (_nChan > 0)
            	products->p1[g] = TOSHORT(_terms[_id].Praw_k[0][g], products->p1Scale, products->p1Offset);
            else
            	products->p1[g] = TOSHORT(0.0, products->p1Scale, products->p1Offset);
            if (_nChan > 1)
            	products->p2[g] = TOSHORT(_terms[_id].Praw_k[1][g], products->p2Scale, products->p2Offset);
            else
            	products->p2[g] = TOSHORT(0.0, products->p2Scale, products->p2Offset);
            if (_nChan > 2 )
            	products->p3[g] = TOSHORT(_terms[_id].Praw_k[2][g], products->p3Scale, products->p3Offset);
            else
            	products->p3[g] = TOSHORT(0.0, products->p3Scale, products->p3Offset);
            if (_nChan > 3)
            	products->p4[g] = TOSHORT(_terms[_id].Praw_k[3][g], products->p4Scale, products->p4Offset);
            else
            	products->p4[g] = TOSHORT(0.0, products->p4Scale, products->p4Offset);            
            
            products->dm[g]  = TOSHORT(_terms[_id].Pant[g], products->dmScale, products->dmOffset);
            products->dbz[g] = TOSHORT(_terms[_id].Dbz[g], products->dbzScale, products->dbzOffset);
            products->vr[g]  = TOSHORT(_terms[_id].Vr[g], products->vrScale, products->vrOffset);
            products->sw[g]  = TOSHORT(_terms[_id].W[g], products->swScale, products->swOffset);
            products->vs[g]  = TOSHORT(_terms[_id].Vs[g], products->vsScale, products->vsOffset);
            products->vl[g]  = TOSHORT(_terms[_id].Vl[g], products->vlScale, products->vlOffset);
            products->ncp[g] = TOSHORT(_terms[_id].Ncp[g], products->ncpScale, products->ncpOffset);
        
            if (g == -1) {
            	std::cout << "vr   "
                << (unsigned short)products->vr[g] << "  "
                << _terms[_id].Vr[g] << "  "
                << products->vrScale << "  "
                << products->vrOffset << "\n";
            	
            	std::cout << "dbz  "
                << (unsigned short)products->dbz[g] << "  "
                << _terms[_id].Dbz[g] << "  "
                << products->dbzScale << "  "
                << products->dbzOffset << "\n";
            }
        }
		        
        // Publish the products
        _productsWriter.publishItem(products);
		
    } else {
        // Oh no, we couldn't get a free products item, so
        // we have to ignore this ray.
        _droppedRays++;
    }
	
    return;
}

////////////////////////////////////////////////////
////////////////////////////////////////////////////
void EldoraProducts::powerRaw(RayData& rays)
{

    // Power raw by channel. Dependent on single/dual prt.
    switch (_dualPrt)
    {
    case false:
        // single prt
        for (int g = 0; g < _gates; g++) {
            for (unsigned int k = 0; k < _nChan; k++) {
                // pick the p out of abp
                double p = rays[0][k]->abp[3*g+2];
                // calculate Praw_k
                if (p > 0.0)
                    _terms[_id].Praw_k[k][g] = 10*log10(p);
                else
                    _terms[_id].Praw_k[k][g] = 0.0;
            }
        }
        break;

    case true:
        // dual prt
        for (int g = 0; g < _gates; g++) {
            for (unsigned int k = 0; k < _nChan; k++) {
                // pick the long and short p out of abp
                double pks = rays[0][k]->abp[3*g+2];
                double pkl = rays[1][k]->abp[3*g+2];
                // calculate Praw_k
                double p = (pks+pkl)/2.0;
                if (p > 0.0)
                    _terms[_id].Praw_k[k][g] = 10*log10(p);
                else
                    _terms[_id].Praw_k[k][g] = 0.0;                    
            }
        }
        break;
    }
}

void EldoraProducts::powerAntenna(RayData& rays)
{
    // Power at the antenna. Not dependent on single/dual prt.
    for (int g = 0; g < _gates; g++) {
        for (unsigned int k = 0; k < _nChan; k++) {
            // calculate Praw_k
            _terms[_id].Pant_k[k][g] = _terms[_id].Praw_k[k][g] - _terms[_id].a_k[k];
        }
    }
}
////////////////////////////////////////////////////
void EldoraProducts::signalPower(RayData& rays)
{
    // Signal power.  Dependent on single/dual prt.
    switch (_dualPrt)
    {
    case false:
        // single prt
        for (int g = 0; g < _gates; g++) {
            double sum = 0.0;
            for (unsigned int k = 0; k < _nChan; k++) {
                // pick the p out of abp
                double p = rays[0][k]->abp[3*g+2];
                p = p - _terms[_id].b10_k[k];
                if (p < 0.0)
                	p = _terms[_id].b10_k[k];
                _terms[_id].Psig_k[k][g] = p;
                sum += _terms[_id].Psig_k[k][g];
            }
            _terms[_id].Psig[g] = sum/_nChan;
        }
        break;

    case true:
        // dual prt
        for (int g = 0; g < _gates; g++) {
            double sumShort = 0.0;
            double sumLong = 0.0;
            for (unsigned int k = 0; k < _nChan; k++) {

                double pks = rays[0][k]->abp[3*g+2];
                pks = pks - _terms[_id].b10_k[k];
                if (pks < 0.0)
                	pks = _terms[_id].b10_k[k];
                _terms[_id].Psigs_k[k][g] = pks;
                sumShort += _terms[_id].Psigs_k[k][g];

                double pkl = rays[1][k]->abp[3*g+2];
                pkl = pkl - _terms[_id].b10_k[k];
                if (pkl < 0.0)
                	pkl = _terms[_id].b10_k[k];
                _terms[_id].Psigl_k[k][g] = pkl;
                sumLong += _terms[_id].Psigl_k[k][g];

                _terms[_id].Psig_k[k][g] = (pks+pkl)/2;
            }

            _terms[_id].Psigs[g] = sumShort/_nChan;
            _terms[_id].Psigl[g] = sumLong/_nChan;
        }
        break;
    }
}

///////////////////////////////////////////////////
void EldoraProducts::totalSignalPower(RayData& rays)
{

    // Total signal power.  Not dependent on single/dual prt.
    for (int g = 0; g < _gates; g++) {
        double sum = 0.0;
        for (unsigned int k = 0; k < _nChan; k++) {
            sum += _terms[_id].Psig_k[k][g]/_terms[_id].a10_k[k];
        }
        _terms[_id].Psigant[g] = sum/_nChan;
    }
}

////////////////////////////////////////////////////
void EldoraProducts::reflectivity(RayData& rays)
{

    // Reflectivity.  Not dependent on single/dual prt.
    for (int g = 0; g < _gates; g++) {
        double p = _terms[_id].Psigant[g];
        if (p > 0.0)  {
            _terms[_id].Dbz[g] =_terms[_id].radarConstant + 10.0*log10(p)
               + _terms[_id].r[g];
	        if (g == 100) {
//	        	std::cout << "_id:" << _id << "  radar constant:" <<
//	        	   _terms[_id].radarConstant << 
//	        	   "  p(logged):" << 10.0*log10(p)
//	        	   << "\n";
	        }
        }
        else {
            //std::cerr << "taking log of negative in relectivity!\n";
            _terms[_id].Dbz[g] = -999.0;
        }
    }
}

////////////////////////////////////////////////////
void EldoraProducts::velocity(RayData& rays)
{
    // Velocity.  Dependent on single/dual prt.
     switch (_dualPrt)
     {
     case false:
         // single prt
         for (int g = 0; g < _gates; g++) {
             double a = atan2(_terms[_id].SumB[0][g], _terms[_id].SumA[0][g]);
             _terms[_id].Vr[g] = _velSign*_terms[_id].Vscale*a;
             _terms[_id].Vs[g] = 0;
             _terms[_id].Vl[g] = 0;
         }
         //std::cout << "SumA:" << _terms[_id].SumA[0][100] 
         //<< "  SumB:" << _terms[_id].SumB[0][100] << "  VScale:"<< _terms[_id].Vscale
         //<< "  Vr:"<<_terms[_id].Vr[100] << "\n";
         break;

     case true:
         // dual prt
         // short and long pulse velocity
         for (int g = 0; g < _gates; g++) {
             _terms[_id].phaseShort[g] = atan2(_terms[_id].SumB[0][g], _terms[_id].SumA[0][g]);
             _terms[_id].Vs[g] = _velSign*_terms[_id].VscaleShort*_terms[_id].phaseShort[g];
             _terms[_id].phaseLong[g] = atan2(_terms[_id].SumB[1][g], _terms[_id].SumA[1][g]);
             _terms[_id].Vl[g] = _velSign*_terms[_id].VscaleLong*_terms[_id].phaseLong[g];
         }
         // unfolded velocity
         unfoldVelocity();
         break;
     }
 }
////////////////////////////////////////////////////
#define fiveFourths (5.0/4.0)
void EldoraProducts::unfoldVelocity() {
    
    // The correction used in the velocity unfolding. Corresponds
    // to the 9 regions in the Loew document.
    static double correction[9] = {
              -M_PI,
              -M_PI*(2+fiveFourths),
             2*M_PI*(1+fiveFourths),
               M_PI*(1+fiveFourths),
               0.0,
              -M_PI*(1+fiveFourths),
            -2*M_PI*(1+fiveFourths),
               M_PI*(2+fiveFourths),
               M_PI
    };
    
    for (int g = 0; g < _gates; g++) {
        // compute the phase difference term
        double deltaPhase = fiveFourths*_terms[_id].phaseShort[g] - _terms[_id].phaseLong[g];
        
        // use the phase difference to find the corresponding correction.
        // See the Loew paper
        int index;
        double n = deltaPhase*4.0/M_PI + 8.0;
        // The table has irregular spacing for the end regions
        if (n < 1.0) {
            index = 0;
        } else {
            if ( 15.0 < n) {
                index = 8;
            } else {
                // for 1.0 <= n <= 15, the spacing is regular
                index = (int)((n+1.0)/2.0);
            }
        }
        double corr = correction[index];
        double sumPhase = fiveFourths*_terms[_id].phaseShort[g] + _terms[_id].phaseLong[g];
       _terms[_id].Vr[g] = _velSign*_terms[_id].VscaleLong*(sumPhase/2.0 + corr);
       if (g == -1) {
    	   std::cout 
    	   << " index:" << index
    	   << " corr:" << corr
    	   << " phaseShort:" << _terms[_id].phaseShort[g]
    	   << " phaseLong:" << _terms[_id].phaseLong[g]
    	   << " VscaleLong:" << _terms[_id].VscaleLong
    	   << " sumPhase:" << sumPhase
    	   << " Vr:" << _terms[_id].Vr[g]
    	   << "\n";
       }
    }
}
////////////////////////////////////////////////////
void EldoraProducts::totalPower(RayData& rays)
{

    // Total power.  Dependent on single/dual prt.
    switch (_dualPrt)
    {
    case false:
        // single prt
        for (int g = 0; g < _gates; g++) {
            double num = 0.0;
            for (unsigned int k = 0; k < _nChan; k++) {
                // pick the p out of abp
                double p = rays[0][k]->abp[3*g+2];
                num += p/_terms[_id].a10_k[k];
            }
            if (num > 0.0)
                _terms[_id].Pant[g] = 10.0*log10(num/_nChan);
            else
                _terms[_id].Pant[g] = 0.0;
        }
        break;

    case true:
        // dual prt
        for (int g = 0; g < _gates; g++) {
            double num = 0.0;
            for (unsigned int k = 0; k < _nChan; k++) {
                // pick the long and short p out of abp
                double pks = rays[0][k]->abp[3*g+2];
                double pkl = rays[1][k]->abp[3*g+2];
                double p = (pks+pkl)/2.0;
                num += p/_terms[_id].a10_k[k];
            }
            if (num > 0.0)
                _terms[_id].Pant[g] = 10.0*log10(num/_nChan);
            else
                _terms[_id].Pant[g] = 0.0;
        }
        break;
    }
}

void EldoraProducts::spectrumWidth(RayData& rays)
{
    // Velocity.  Dependent on single/dual prt.
     switch (_dualPrt)
     {
     case false:
         // single prt
         for (int g = 0; g < _gates; g++) {
             double asum = _terms[_id].SumA[0][g] / _nChan;
             double bsum = _terms[_id].SumB[0][g] / _nChan;
             double denom = sqrt(asum*asum + bsum*bsum);
             double term;
             if (_terms[_id].Psig[g] >= denom) {
                 term = _terms[_id].Psig[g] / denom;
             } else {
            	 term = 1.0;
             }
             _terms[_id].W[g] = _terms[_id].Wscale*sqrt(log(term));
         }
         break;

     case true:
         // dual prt
         for (int g = 0; g < _gates; g++) {
             double asumShort = _terms[_id].SumA[0][g] / _nChan;
             double bsumShort = _terms[_id].SumB[0][g] / _nChan;
             double denomShort = sqrt(square(asumShort)+ square(bsumShort));

             double asumLong = _terms[_id].SumA[1][g] / _nChan;
             double bsumLong = _terms[_id].SumB[1][g] / _nChan;
             double denomLong = sqrt(square(asumLong) + square(bsumLong));
             double termShort;
             double termLong;
             if (_terms[_id].Psigs[g] >= denomShort && _terms[_id].Psigl[g] >= denomLong ) {
                  termShort = _terms[_id].Psigs[g] / denomShort;
                  termLong = _terms[_id].Psigl[g] / denomLong;
             } else {
            	  termShort = 1.0;
            	  termLong = 1.0;
             }           
              
              _terms[_id].W[g] = _terms[_id].WscaleShort*sqrt(log(termShort)) + _terms[_id].WscaleLong*sqrt(log(termLong));

          }
         break;
     }
 }

void EldoraProducts::ncp(RayData& rays)
{
    // Normalized coherent power. Dependent on single/dual prt.
    switch (_dualPrt)
    {
    case false:
        for (int g = 0; g < _gates; g++) {
            _terms[_id].Ncp[g] = sqrt(square(_terms[_id].SumA[0][g]/_nChan)+square(_terms[_id].SumB[0][g]/_nChan))/(_terms[_id].SumP[0][g]/_nChan);
        }
        break;
    case true:
        for (int g = 0; g < _gates; g++) {
        	_terms[_id].Ncp[g] = 0.0;
        	
            _terms[_id].Ncp[g] += sqrt(square(_terms[_id].SumA[0][g]/_nChan) + square(_terms[_id].SumB[0][g]/_nChan))
            		             / (_terms[_id].SumP[0][g]/_nChan);
            
            _terms[_id].Ncp[g] += sqrt(square(_terms[_id].SumA[1][g]/_nChan)+square(_terms[_id].SumB[1][g]/_nChan))
                                 /(_terms[_id].SumP[1][g]/_nChan);
            
            _terms[_id].Ncp[g] /= 2.0;
        }        
        break;
    }
    
}

////////////////////////////////////////////////////
void EldoraProducts::computeSums(RayData& rays) {
    
    int nPrts = _dualPrt? 2:1;
    
    for (int prtId = 0; prtId < nPrts; prtId++) {
        for (int g = 0; g < _gates; g++) {
            _terms[_id].SumA[prtId][g] = 0.0;
            _terms[_id].SumB[prtId][g] = 0.0;
            _terms[_id].SumP[prtId][g] = 0.0;
            for (int k = 0; k < _nChan; k++) {
                _terms[_id].SumA[prtId][g] += rays[prtId][k]->abp[3*g+0];
                _terms[_id].SumB[prtId][g] += rays[prtId][k]->abp[3*g+1];
                _terms[_id].SumP[prtId][g] += rays[prtId][k]->abp[3*g+2];                
            }
        }
    }
}
////////////////////////////////////////////////////

int EldoraProducts::numRays()
{
    int n = _rays;
    _rays = 0;
    return n;
}

////////////////////////////////////////////////////
void EldoraProducts::initTerms(RayData& rays)
{
    if (_termsInitialized[_id])
        return;

    // Initialize the terms
    _terms[_id].init(_nChan,
    		    _gates,
                rays[0][0]->hskp.radarConstant,
                rays[0][0]->hskp.xBandGain,
                rays[0][0]->hskp.noisePower,
                rays[0][0]->hskp.lnaLoss,
                rays[0][0]->hskp.freqs,
                rays[0][0]->hskp.rxGain,
                rays[0][0]->hskp.prt,
                rays[0][0]->hskp.prt,
                rays[0][0]->hskp.prtLong,
                rays[0][0]->hskp.firstGateDistance,
                rays[0][0]->hskp.cellWidth[0],
                _dualPrt);

    _scaling.dmScale  = 100.0;  
    _scaling.dmBias   = 13500.0;
    
    _scaling.pScale   = 100.0;
    _scaling.pBias    = 9000.0;
    
    _scaling.dbzScale = rays[0][0]->hskp.dbzScale;
    _scaling.dbzBias  = rays[0][0]->hskp.dbzBias;

    _scaling.swScale  = rays[0][0]->hskp.swScale;
    _scaling.swBias   = rays[0][0]->hskp.swBias;

    _scaling.ncpScale = rays[0][0]->hskp.ncpScale;
    _scaling.ncpBias  = rays[0][0]->hskp.ncpBias;

    _scaling.vsScale  = rays[0][0]->hskp.vsScale;
    _scaling.vsBias   = rays[0][0]->hskp.vsBias;

    _scaling.vlScale  = rays[0][0]->hskp.vlScale;
    _scaling.vlBias   = rays[0][0]->hskp.vlBias;

    _scaling.vrScale  = rays[0][0]->hskp.vrScale;
    _scaling.vrBias   = rays[0][0]->hskp.vrBias;

    _termsInitialized[_id] = true;
}

////////////////////////////////////////////////////
void EldoraProducts::initProducts(EldoraDDS::Products* p, 
                                  EldoraDDS::Housekeeping& hskp,
                                  int gates)
{
    // Resize the ray lengths.
    p->p1.length(gates);
    p->p2.length(gates);
    p->p3.length(gates);
    p->p4.length(gates);
    p->vr.length(gates);
    p->vs.length(gates);
    p->vl.length(gates);
    p->dm.length(gates);
    p->dbz.length(gates);
    p->sw.length(gates);
    p->ncp.length(gates);

    p->p1Scale = _scaling.pScale;
    p->p1Offset = _scaling.pBias;

    p->p2Scale = _scaling.pScale;
    p->p2Offset = _scaling.pBias;

    p->p3Scale = _scaling.pScale;
    p->p3Offset = _scaling.pBias;

    p->p4Scale = _scaling.pScale;
    p->p4Offset = _scaling.pBias;

    p->dmScale = _scaling.dmScale;
    p->dmOffset = _scaling.dmBias;

    p->dbzScale = _scaling.dbzScale;
    p->dbzOffset = _scaling.dbzBias;

    p->vrScale = _scaling.vrScale;
    p->vrOffset = _scaling.vrBias;

    p->vsScale = _scaling.vsScale;
    p->vsOffset = _scaling.vsBias;

    p->vlScale = _scaling.vlScale;
    p->vlOffset = _scaling.vsBias;

    p->swScale = _scaling.swScale;
    p->swOffset = _scaling.swBias;

    p->ncpScale = _scaling.ncpScale;
    p->ncpOffset = _scaling.ncpBias;
    
    p->hskp = hskp;
}
////////////////////////////////////////////////////
void ProductsTerms::init(int nChan,
						 int gates,
                         float radConstant,
                         float xBandGain,
                         float pNoise,
                         float lnaLoss,
                         float freqs[4],
                         float rxGain[4],
                         float prt,
                         float prtShort,
                         float prtLong,
                         float firstGate,
                         float cellWidth,
                         bool dualPrt)
{

    // resize vectors. 
    // Some need space for _nChan channels
    // Some need space for gates
    // some need space for channels and gates
    lambda_k.resize(nChan);
    a_k.resize(nChan);
    a10_k.resize(nChan);
    b_k.resize(nChan);
    b10_k.resize(nChan);
    Vscale_k.resize(nChan);
    phaseShort.resize(gates);
    phaseLong.resize(gates);
    SumA.resize(2);    // for short and long prts
    SumB.resize(2);    // for short and long prts
    SumP.resize(2);    // for short and long prts
    r.resize(gates);
    Praw_k.resize(nChan);
    Pant_k.resize(nChan);
    Pant.resize(gates);
    Psig_k.resize(nChan);
    Psig.resize(gates);
    Psigant.resize(gates);
    //V_k.resize(_nChan);
    Psigs_k.resize(nChan);
    Psigl_k.resize(nChan);
    Psigs.resize(gates);
    Psigl.resize(gates);
    Thetas.resize(gates);
    Thetal.resize(gates);
    DeltaTheta.resize(gates);
    Dbz.resize(gates);
    W.resize(gates);
    Ncp.resize(gates);
    Vr.resize(gates);
    Vs.resize(gates);
    Vl.resize(gates);

    for (unsigned int k = 0; k < nChan; k++) {
        Praw_k[k].resize(gates);
        Pant_k[k].resize(gates);
        Psig_k[k].resize(gates);
        Psigs_k[k].resize(gates);
        Psigl_k[k].resize(gates);
        //V_k[k].resize(gates);
    }
    
    for (unsigned int prtId = 0; prtId < 2; prtId++) {
        SumA[prtId].resize(gates);
        SumB[prtId].resize(gates);
        SumP[prtId].resize(gates);
    }

    radarConstant = fabs(radConstant);

    // calculate constants
    
    // range correction
    for (int g = 0; g < gates; g++) {
        r[g] = 20.0*log10(((g+1)*cellWidth + firstGate) / 1000.0);
    }

    lambda = 0.0;
    for (int k = 0; k < nChan; k++) {
        lambda_k[k] = 3.0e8/(freqs[k]*1.0e9);
        lambda += lambda_k[k];
    }
    lambda = lambda/nChan;

    for (int k = 0; k < nChan; k++) {
        
    	a_k[k] = xBandGain + rxGain[k] - lnaLoss;
        a10_k[k] = pow(10.0, a_k[k]/10.0);

        b_k[k] = xBandGain + rxGain[k] - lnaLoss + pNoise;
        b10_k[k] = pow(10.0, b_k[k]/10);
    }

    for (int k = 0; k < nChan; k++) {
        Vscale_k[k] = lambda_k[k]/(4*M_PI*prt/1000.0);
    }

    Vscale = lambda/(4*M_PI*prt/1000.0);
    Wscale = lambda/(sqrt(8.0)*M_PI*prt/1000.0);

    if (!dualPrt)
        return;

    VscaleShort = lambda/(4*M_PI*prtShort/1000.0);
    WscaleShort = lambda/(sqrt(8.0)*M_PI*prtShort/1000.0);

    VscaleLong = lambda/(4*M_PI*prtLong/1000.0);
    WscaleLong = lambda/(sqrt(8.0)*M_PI*prtLong/1000.0);

}
////////////////////////////////////////////////////

