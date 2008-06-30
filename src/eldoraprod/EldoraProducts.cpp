#include "EldoraProducts.h"
#include <iostream>
#include <math.h>

/// Macro used to apply scale and bias to convert to 16 bit 
/// integer int for on-the-wire transfer
#define TOSHORT(data, scale, bias) ((short)(data*scale+bias))

/// A squaring function
#define square(a) (a*a)

////////////////////////////////////////////////////
EldoraProducts::EldoraProducts(DDSPublisher& publisher,
                               std::string productsTopic,
                               bool dualPrt,
                               bool reverseVelocity) :
    _rays(0), _publisher(publisher), _productsTopic(productsTopic),
            _productsWriter(publisher, productsTopic), _droppedRays(0),
            _dualPrt(dualPrt), _termsInitialized(false)
{
    if (reverseVelocity)
        _velSign = -1;
    else
        _velSign = 1;

}

////////////////////////////////////////////////////
EldoraProducts::~EldoraProducts()
{

}

////////////////////////////////////////////////////
void EldoraProducts::newRayData(std::vector<std::vector<EldoraDDS::Ray*> >& rays)
{

    static bool first = true;
    if (first) {
        std::cout << "Products hase received first ray " 
        << " prt:"
        << rays[0][0]->hskp.prt 
        << " prtLong:"
        << rays[0][0]->hskp.prtLong 
        << "  gate spacing:" 
        << rays[0][0]->hskp.cellWidth[0] 
        << "\n";
        first = false;
    }
    _rays++;

    // make sure that we got the right number of prts
    if ((_dualPrt && rays.size() != 2) || (!_dualPrt && rays.size() != 1)) {
        std::cout << "prt mode does not match the number of prt ids received\n";
        _droppedRays++;
        return;
    }

    _gates = rays[0][0]->abp.length()/3;
    initTerms(rays);

    // We have four matched rays for each prt 
	
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
            products->p1[g] = TOSHORT(_terms.Praw_k[0][g], products->p1Scale, products->p1Offset);
            products->p2[g] = TOSHORT(_terms.Praw_k[1][g], products->p2Scale, products->p2Offset);
            products->p3[g] = TOSHORT(_terms.Praw_k[2][g], products->p3Scale, products->p3Offset);
            products->p4[g] = TOSHORT(_terms.Praw_k[3][g], products->p4Scale, products->p4Offset);
            products->dm[g] = TOSHORT(_terms.Pant[g], products->dmScale, products->dmOffset);
            products->dbz[g] = TOSHORT(_terms.Dbz[g], products->dbzScale, products->dbzOffset);
            products->vr[g] = TOSHORT(_terms.Vr[g], products->vrScale, products->vrOffset);
            products->sw[g] = TOSHORT(_terms.W[g], products->swScale, products->swOffset);
            products->vs[g] = TOSHORT(_terms.Vs[g], products->vsScale, products->vsOffset);
            products->vl[g] = TOSHORT(_terms.Vl[g], products->vlScale, products->vlOffset);
            products->ncp[g] = TOSHORT(_terms.Ncp[g], products->ncpScale, products->ncpOffset);
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
void EldoraProducts::powerRaw(RayData& rays)
{

    // Power raw by channel. Dependent on single/dual prt.
    switch (_dualPrt)
    {
    case false:
        // single prt
        for (int g = 0; g < _gates; g++) {
            for (unsigned int k = 0; k < 4; k++) {
                // pick the p out of abp
                double p = rays[0][k]->abp[3*g+2];
                // calculate Praw_k
                if (p > 0.0)
                    _terms.Praw_k[k][g] = 10*log10(p);
                else
                    _terms.Praw_k[k][g] = 0.0;
            }
        }
        break;

    case true:
        // dual prt
        for (int g = 0; g < _gates; g++) {
            for (unsigned int k = 0; k < 4; k++) {
                // pick the long and short p out of abp
                double pks = rays[0][k]->abp[3*g+2];
                double pkl = rays[1][k]->abp[3*g+2];
                // calculate Praw_k
                double p = (pks+pkl)/2.0;
                if (p > 0.0)
                    _terms.Praw_k[k][g] = 10*log10(p);
                else
                    _terms.Praw_k[k][g] = 0.0;                    
            }
        }
        break;
    }
}

////////////////////////////////////////////////////
void EldoraProducts::powerAntenna(RayData& rays)
{
    // Power at the antenna. Not dependent on single/dual prt.
    for (int g = 0; g < _gates; g++) {
        for (unsigned int k = 0; k < 4; k++) {
            // calculate Praw_k
            _terms.Pant_k[k][g] = _terms.Praw_k[k][g] - _terms.a_k[k];
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
            for (unsigned int k = 0; k < 4; k++) {
                // pick the p out of abp
                double p = rays[0][k]->abp[3*g+2];
                num += p/_terms.a10_k[k];
            }
            if (num > 0.0)
                _terms.Pant[g] = 10.0*log10(num/4.0);
            else
                _terms.Pant[g] = 0.0;
        }
        break;

    case true:
        // dual prt
        for (int g = 0; g < _gates; g++) {
            double num = 0.0;
            for (unsigned int k = 0; k < 4; k++) {
                // pick the long and short p out of abp
                double pks = rays[0][k]->abp[3*g+2];
                double pkl = rays[1][k]->abp[3*g+2];
                double p = (pks+pkl)/2.0;
                num += p/_terms.a10_k[k];
            }
            if (num > 0.0)
                _terms.Pant[g] = 10.0*log10(num/4.0);
            else
                _terms.Pant[g] = 0.0;
        }
        break;
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
            for (unsigned int k = 0; k < 4; k++) {
                // pick the p out of abp
                double p = rays[0][k]->abp[3*g+2];
                _terms.Psig_k[k][g] = p - _terms.b10_k[k];
                sum += _terms.Psig_k[k][g];
            }
            _terms.Psig[g] = sum/4.0;
        }
        break;

    case true:
        // dual prt
        for (int g = 0; g < _gates; g++) {
            double sumShort = 0.0;
            double sumLong = 0.0;
            for (unsigned int k = 0; k < 4; k++) {

                double pks = rays[0][k]->abp[3*g+2];
                _terms.Psigs_k[k][g] = pks - _terms.b10_k[k];
                sumShort += _terms.Psigs_k[k][g];

                double pkl = rays[1][k]->abp[3*g+2];
                _terms.Psigl_k[k][g] = pkl - _terms.b10_k[k];
                sumLong += _terms.Psigl_k[k][g];

                _terms.Psig_k[k][g] = (pks+pkl)/2 - _terms.b10_k[k];
            }

            _terms.Psigs[g] = sumShort/4.0;
            _terms.Psigl[g] = sumLong/4.0;
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
        for (unsigned int k = 0; k < 4; k++) {
            sum += _terms.Psig_k[k][g]/_terms.a10_k[k];
        }
        _terms.Psigant[g] = sum/4.0;
    }
}

////////////////////////////////////////////////////
void EldoraProducts::reflectivity(RayData& rays)
{

    // Reflectivity.  Not dependent on single/dual prt.
    for (int g = 0; g < _gates; g++) {
        double p = _terms.Psigant[g];
        if (p > 0.0) 
            _terms.Dbz[g] =_terms.radarConstant + 10.0*log10(p)
                + _terms.r[g];
        else
            _terms.Dbz[g] = -35.0;
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
             double a = atan2(_terms.SumB[0][g], _terms.SumA[0][g]);
             _terms.Vr[g] = _velSign*_terms.Vscale*a;
             _terms.Vs[g] = 0;
             _terms.Vl[g] = 0;
         }
         //std::cout << "SumA:" << _terms.SumA[0][100] 
         //<< "  SumB:" << _terms.SumB[0][100] << "  VScale:"<< _terms.Vscale
         //<< "  Vr:"<<_terms.Vr[100] << "\n";
         break;

     case true:
         // dual prt
         // short and long pulse velocity
         for (int g = 0; g < _gates; g++) {
             _terms.phaseShort[g] = atan2(_terms.SumB[0][g], _terms.SumA[0][g]);
             _terms.Vs[g] = _velSign*_terms.VscaleShort*_terms.phaseShort[g];
             _terms.phaseLong[g] = atan2(_terms.SumB[1][g], _terms.SumA[1][g]);
             _terms.Vl[g] = _velSign*_terms.VscaleLong*_terms.phaseLong[g];
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
        double deltaPhase = fiveFourths*_terms.phaseShort[g] - _terms.phaseLong[g];
        
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
        double sumPhase = fiveFourths*_terms.phaseShort[g] + _terms.phaseLong[g];
       _terms.Vr[g] = _terms.VscaleLong*(sumPhase/2.0 + corr);
    }
}
////////////////////////////////////////////////////
void EldoraProducts::spectrumWidth(RayData& rays)
{
    // Velocity.  Dependent on single/dual prt.
     switch (_dualPrt)
     {
     case false:
         // single prt
         for (int g = 0; g < _gates; g++) {
             double asum = _terms.SumA[0][g] / 4.0;
             double bsum = _terms.SumB[0][g] / 4.0;
             double denom = sqrt(asum*asum + bsum*bsum);
             if (_terms.Psig[g] >= denom) {
                 double term = _terms.Psig[g] / denom;
                 _terms.W[g] = _terms.Wscale*sqrt(log(term));
             } else {
                 // @tod need to get the nyquist velocity in here
                 _terms.W[g] = _terms.Vscale*M_PI;
             }
         }
         break;

     case true:
         // dual prt
         for (int g = 0; g < _gates; g++) {
             double asumShort = _terms.SumA[0][g] / 4.0;
             double bsumShort = _terms.SumB[0][g] / 4.0;
             double denomShort = sqrt(square(asumShort)+ square(bsumShort));

             double asumLong = _terms.SumA[1][g] / 4.0;
             double bsumLong = _terms.SumB[1][g] / 4.0;
             double denomLong = sqrt(square(asumLong) + square(bsumLong));
             
              if (_terms.Psigs[g] >= denomShort && _terms.Psigl[g] >= denomLong ) {
                  double termShort = _terms.Psigs[g] / denomShort;
                  double termLong = _terms.Psigl[g] / denomLong;
                  _terms.W[g] = _terms.WscaleShort*sqrt(log(termShort)) + _terms.WscaleLong*sqrt(log(termLong));
              } else {
                  // @tod need to get the nyquist velocity in here
                  _terms.W[g] = ((_terms.VscaleShort + _terms.VscaleLong)/2.0)*M_PI;
              }
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
            _terms.Ncp[g] = sqrt(square(_terms.SumA[0][g]/4.0)+square(_terms.SumB[0][g]/4.0))/(_terms.SumP[0][g]/4.0);
        }
        break;
    case true:
        for (int g = 0; g < _gates; g++) {
            _terms.Ncp[g] = sqrt(square(_terms.SumA[0][g]/4.0)+square(_terms.SumB[0][g]/4.0))/(_terms.SumP[0][g]/4.0);
            _terms.Ncp[g] += sqrt(square(_terms.SumB[1][g]/4.0)+square(_terms.SumB[1][g]/4.0))/(_terms.SumP[1][g]/4.0);
            _terms.Ncp[g] /= 2.0;
        }        
        break;
    }
    
}

////////////////////////////////////////////////////
void EldoraProducts::computeSums(RayData& rays) {
    
    int nPrts = _dualPrt? 2:1;
    
    for (int prtId = 0; prtId < nPrts; prtId++) {
        for (int g = 0; g < _gates; g++) {
            _terms.SumA[prtId][g] = 0.0;
            _terms.SumB[prtId][g] = 0.0;
            _terms.SumP[prtId][g] = 0.0;
            for (int k = 0; k < 4; k++) {
                _terms.SumA[prtId][g] += rays[prtId][k]->abp[3*g+0];
                _terms.SumB[prtId][g] += rays[prtId][k]->abp[3*g+1];
                _terms.SumP[prtId][g] += rays[prtId][k]->abp[3*g+2];                
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
    if (_termsInitialized)
        return;

    // Initialize the terms
    _terms.init(_gates,
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
    _scaling.dmBias   = 11500.0;
    
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

    _termsInitialized = true;
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
void ProductsTerms::init(int gates,
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
    // Some need space for 4 channels
    // Some need space for gates
    // some need space for channels and gates
    lambda_k.resize(4);
    a_k.resize(4);
    a10_k.resize(4);
    b_k.resize(4);
    b10_k.resize(4);
    Vscale_k.resize(4);
    phaseShort.resize(gates);
    phaseLong.resize(gates);
    SumA.resize(2);    // for short and long prts
    SumB.resize(2);    // for short and long prts
    SumP.resize(2);    // for short and long prts
    r.resize(gates);
    Praw_k.resize(4);
    Pant_k.resize(4);
    Pant.resize(gates);
    Psig_k.resize(4);
    Psig.resize(gates);
    Psigant.resize(gates);
    //V_k.resize(4);
    Psigs_k.resize(4);
    Psigl_k.resize(4);
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

    for (unsigned int k = 0; k < 4; k++) {
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
    for (unsigned int k = 0; k < 4; k++) {
        lambda_k[k] = 3.0e8/(freqs[k]*1.0e9);
        lambda += lambda_k[k];
    }
    lambda = lambda/4.0;

    for (unsigned int k = 0; k < 4; k++) {
        a_k[k] = xBandGain + rxGain[k] - lnaLoss;
        a10_k[k] = pow(10.0, a_k[k]/10.0);

        b_k[k] = xBandGain + rxGain[k] + pNoise;
        b10_k[k] = pow(10.0, b_k[k]/10);
    }

    for (unsigned int k = 0; k < 4; k++) {
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

