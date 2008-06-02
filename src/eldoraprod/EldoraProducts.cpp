#include "EldoraProducts.h"
#include <iostream>
#include <math.h>
#define TOSHORT(data, gain, offset) ((short)(((data)*(gain))-(offset)))

////////////////////////////////////////////////////
EldoraProducts::EldoraProducts(DDSPublisher& publisher,
                               std::string productsTopic,
                               bool dualPrt) :
    _rays(0), _publisher(publisher), _productsTopic(productsTopic),
            _productsWriter(publisher, productsTopic), _droppedRays(0),
            _dualPrt(dualPrt), _termsInitialized(false)
{

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
        std::cout << "xBandGain:" << rays[0][0]->hskp.xBandGain << " prt:"
                << rays[0][0]->hskp.prt << " prtLong:"
                << rays[0][0]->hskp.prtLong << "\n";
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
    Products* products = _productsWriter.getEmptyItem();
    if (products) {

        // The abp rays are three times as long as the
        // product rays. 
        int gates = rays[0][0]->abp.length()/3;

        // initiallize the fixed fields in Products, and resize the vectors.
        initProducts(products, gates);

        // This is where the real work gets done
        // compute the varius terms found in Eric's paper
        powerRaw(rays);
        powerAntenna(rays);
        totalPower(rays);
        signalPower(rays);
        totalSignalPower(rays);
        reflectivity(rays);
        velocity(rays);
        spectralWidth(rays);

        // fill in the product ray
        // first transfer ray metadata
        products->radarId = rays[0][0]->radarId;
        products->timestamp = rays[0][0]->hskp.rayNum;
        products->rotAngle = rays[0][0]->hskp.radarRotAngle;

        // transfer the products.
        for (int g = 0; g < gates; g++) {
            products->p1[g] = TOSHORT(_terms.Praw_k[0][g], products->p1Scale, products->p1Offset);
            products->p2[g] = TOSHORT(_terms.Praw_k[1][g], products->p2Scale, products->p2Offset);
            products->p3[g] = TOSHORT(_terms.Praw_k[2][g], products->p3Scale, products->p3Offset);
            products->p4[g] = TOSHORT(_terms.Praw_k[3][g], products->p4Scale, products->p4Offset);
            products->dm[g] = TOSHORT(_terms.Pant[g], products->dmScale, products->dmOffset);
            products->dbz[g] = TOSHORT(_terms.Dbz[g], products->dbzScale, products->dbzOffset);
            products->vr[g] = TOSHORT(_terms.Vr[g], products->vrScale, products->vrOffset);
            products->sw[g] = TOSHORT(_terms.W[g], products->swScale, products->swOffset);
            
            products->vs[g] = TOSHORT(0.0, products->vsScale, products->vsOffset);
            products->vl[g] = TOSHORT(0.0, products->vlScale, products->vlOffset);
            products->ncp[g] = TOSHORT(0.0, products->ncpScale, products->ncpOffset);
        }
        // Publish the products.
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

////////////////////////////////////////////////////
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
            _terms.Dbz[g] = 0.0;
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
             double asum = 0.0;
             double bsum = 0.0;
             for (unsigned int k = 0; k < 4; k++) {
                 // pick the a and b out of abp
                 double a = rays[0][k]->abp[3*g+0];
                 double b = rays[0][k]->abp[3*g+1];
                 //_terms.V_k[k][g] = _terms.Vscale_k[k]*atan2(b, a);
                 asum += a;
                 bsum += b;
             }
             double a = atan2(bsum, asum);
             _terms.Vr[g] = _terms.Vscale*a;
             _terms.Vs[g] = 0;
             _terms.Vl[g] = 0;
         }
         break;

     case true:
         // dual prt
         for (int g = 0; g < _gates; g++) {
             double asumShort = 0.0;
             double asumLong = 0.0;
             double bsumShort = 0.0;
             double bsumLong = 0.0;
             for (unsigned int k = 0; k < 4; k++) {
                 // pick the a and b out of abp
                 asumShort += rays[0][k]->abp[3*g+0];
                 bsumShort += rays[0][k]->abp[3*g+1];
                 asumLong += rays[1][k]->abp[3*g+0];
                 bsumLong += rays[1][k]->abp[3*g+1];
             }
             /// @todo put in the velocity unfolding algorithm
             _terms.Vr[g] = 0.0;
             _terms.Vs[g] = _terms.Vscale*atan2(bsumShort, asumShort);
             _terms.Vl[g] = _terms.Vscale*atan2(bsumLong, asumLong);
         }
         break;
     }
 }

////////////////////////////////////////////////////
void EldoraProducts::spectralWidth(RayData& rays)
{
    // Velocity.  Dependent on single/dual prt.
     switch (_dualPrt)
     {
     case false:
         // single prt
         for (int g = 0; g < _gates; g++) {
             double asum = 0.0;
             double bsum = 0.0;
             for (unsigned int k = 0; k < 4; k++) {
                 // pick the a and b out of abp
                 asum += rays[0][k]->abp[3*g+0];
                 bsum += rays[0][k]->abp[3*g+1];
             }
             asum = asum/4.0;
             bsum = bsum/4.0;
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
             _terms.W[g] = 0.0;
         }
         break;
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
                _dualPrt);

    _termsInitialized = true;
}

////////////////////////////////////////////////////
void EldoraProducts::initProducts(EldoraDDS::Products* p,
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

    p->p1Scale = 100.0;
    p->p1Offset = 9000.0;

    p->p2Scale = 100.0;
    p->p2Offset = 9000.0;

    p->p3Scale = 100.0;
    p->p3Offset = 9000.0;

    p->p4Scale = 100.0;
    p->p4Offset = 9000.0;

    p->dmScale = 100.0;
    p->dmOffset = 11500.0;

    p->dbzScale = 10.0;
    p->dbzOffset = 350.0;

    p->vrScale = 100.0;
    p->vrOffset = 0.0;

    p->vsScale = 100.0;
    p->vsOffset = 0.0;

    p->vlScale = 100.0;
    p->vlOffset = 0.0;

    p->swScale = 43.0;
    p->swOffset = 0.0;

    p->ncpScale = 1000.0;
    p->ncpOffset = 0.0;
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

    radarConstant = fabs(radConstant);

    // calculate constants
    for (int g = 0; g < gates; g++) {
        /// @todo use the real gate spacing here!
        r[g] = log10((g*100.0 + 50.0) / 1000.0);
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

