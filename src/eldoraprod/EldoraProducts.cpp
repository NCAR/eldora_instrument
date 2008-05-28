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

        // initiallize the fixed fields in Products.
        initProducts(products);

        rawPower(rays);
        
        int prt = 0;

        // transfer ray metadata
        products->radarId = rays[prt][0]->radarId;
        products->timestamp = rays[prt][0]->hskp.rayNum;
        products->rotAngle = rays[prt][0]->hskp.radarRotAngle;

        // The abp rays are three times as long as the
        // product beams. 
        int productsLength = rays[prt][0]->abp.length()/3;
        // Resize the beam lengths.
        products->p1.length(productsLength);
        products->p2.length(productsLength);
        products->p3.length(productsLength);
        products->p4.length(productsLength);
        products->vr.length(productsLength);
        products->vs.length(productsLength);
        products->vl.length(productsLength);
        products->dm.length(productsLength);
        products->dbz.length(productsLength);
        products->sw.length(productsLength);
        products->ncp.length(productsLength);

        // Compute the products.
        for (int gate = 0; gate < productsLength; gate++) {
            int p = 3*gate;
            // temprary hack: Sum all four channel powers to create
            // dbz.
            double dbz = 0.0;
            for (unsigned int f = 0; f < 4; f++) {
                dbz += rays[prt][f]->abp[p+2];
            }

            products->dm[gate]
                    = TOSHORT(dbz/(((gate+1.0)*(gate+1.0))/100.0), products->dmGain, products->dmOffset);

            products->dbz[gate] = TOSHORT(dbz, products->dbzGain, products->dbzOffset)*100-70;

            products->p1[gate] = TOSHORT(_terms.Praw_k[0][gate], products->p1Gain, products->p1Offset);
            products->p2[gate] = TOSHORT(_terms.Praw_k[1][gate], products->p2Gain, products->p2Offset);
            products->p3[gate] = TOSHORT(_terms.Praw_k[2][gate], products->p3Gain, products->p3Offset);
            products->p4[gate] = TOSHORT(_terms.Praw_k[3][gate], products->p4Gain, products->p4Offset);

            products->vr[gate]
                    = TOSHORT((gate-productsLength/2.0), products->vrGain, products->vrOffset);
            products->vs[gate]
                    = TOSHORT((gate-productsLength/2.0)/10.0, products->vsGain, products->vsOffset);
            products->vl[gate]
                    = TOSHORT(gate*(gate-productsLength/2.0)/productsLength, products->vlGain, products->vlOffset);
            products->sw[gate] = TOSHORT(1.0*gate, products->swGain, products->swOffset);
            products->ncp[gate]
                    = TOSHORT(1.0/((gate+1.0)*(gate+1.0)), products->ncpGain, products->ncpOffset);
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
void EldoraProducts::rawPower(RayData& rays)
{
    switch (_dualPrt)
    {
    case false:
        // single prt
        for (int g = 0; g < _gates; g++) {
            for (unsigned int k = 0; k < 4; k++) {
                // pick the p out of abp
                double p = rays[0][k]->abp[3*g+2];
                // calculate Praw_k
                _terms.Praw_k[k][g] = 10*log10(p);
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
                _terms.Praw_k[k][g] = 10*log10((pks+pkl)/2.0);
            }
        }
        break;
    }
}

////////////////////////////////////////////////////
void EldoraProducts::antennaPower(RayData& rays)
{
    switch (_dualPrt)
    {
    case false:
        // single prt
        for (int g = 0; g < _gates; g++) {
            for (unsigned int k = 0; k < 4; k++) {
                // pick the p out of abp
                double p = rays[0][k]->abp[3*g+2];
                // calculate Praw_k
                _terms.Praw_k[k][g] = 10*log10(p);
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
                _terms.Praw_k[k][g] = 10*log10((pks+pkl)/2.0);
            }
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
    
    /// The x band gain.
    double xBandGain;
    /// The channel frequencies.
    double freqs[4];
    /// The receiver gains.
    double rxGain[4];
    /// The input losses.
    double lossIn[4];
    /// The prt in single mode.
    double prt;
    /// The short prt in dual mode.
    double prtShort;
    /// The long prt in dual mode.
    double prtLong;

    // Initialize the terms
    _terms.init(_gates,
                xBandGain,
                freqs,
                rxGain,
                lossIn,
                prt,
                prtShort,
                prtLong);
    
    _termsInitialized = true;
}

////////////////////////////////////////////////////
void EldoraProducts::initProducts(EldoraDDS::Products* p)
{

    p->p1Gain = 1.0e2;
    p->p1Offset = 9000.0;

    p->p2Gain = 1.0e2;
    p->p2Offset = 9000.0;

    p->p3Gain = 1.0e2;
    p->p3Offset = 9000.0;

    p->p4Gain = 1.0e2;
    p->p4Offset = 9000.0;

    p->dmGain = 1000.0;
    p->dmOffset = 0.0;

    p->dbzGain = 1000.0;
    p->dbzOffset = 0.0;

    p->vrGain = 100.0;
    p->vrOffset = 0.0;

    p->vsGain = 100.0;
    p->vsOffset = 0.0;

    p->vlGain = 100.0;
    p->vlOffset = 0.0;

    p->swGain = 100.0;
    p->swOffset = 32768.0;

    p->ncpGain = 1000.0;
    p->ncpOffset = 32768.0;
}
////////////////////////////////////////////////////
void ProductsTerms::init(int gates, 
                         double xBandGain,
                         double freqs[4], 
                         double rxGain[4], 
                         double lossIn[4],
                         double prt,
                         double prtShort,
                         double prtLong) {

    Praw_k.resize(4);
    Pant_k.resize(4);
    Pant.resize(gates);
    Psig_k.resize(4);
    Psig.resize(gates);
    V_k.resize(4);
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
    Vs.resize(gates);
    Vr.resize(gates);

    for (unsigned int c = 0; c < 4; c++) {
        Praw_k[c].resize(gates);
        Pant_k[c].resize(gates);
        Psig_k[c].resize(gates);
        Psigs_k[c].resize(gates);
        Psigl_k[c].resize(gates);
    }
 
}
////////////////////////////////////////////////////

