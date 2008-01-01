#include "EldoraProducts.h"
#include <iostream>
////////////////////////////////////////////////////
EldoraProducts::EldoraProducts(
        DDSPublisher& publisher,
            std::string productsTopic) :
    _pulses(0), _publisher(publisher), _productsTopic(productsTopic),
            _productsWriter(publisher, productsTopic), _droppedPulses(0) {
            
}

////////////////////////////////////////////////////
EldoraProducts::~EldoraProducts() {

}

////////////////////////////////////////////////////
void EldoraProducts::newPulseData(
        std::vector<EldoraDDS::Pulse*>& pulses) {

    _pulses++;

    // We have four matched pulses. 
    Products* products = _productsWriter.getEmptyItem();
    if (products) {

        // transfer beam metadata
        products->radarId = pulses[0]->radarId;
        products->timestamp = pulses[0]->timestamp;
        // The abp pulses are three times as long as the
        // product beams. 
        int productsLength = pulses[0]->abp.length()/3;
        // Resize the beam lengths.
        products->p1.length(productsLength);
        products->p2.length(productsLength);
        products->p3.length(productsLength);
        products->p4.length(productsLength);
        products->dbm.length(productsLength);
        products->dbz.length(productsLength);
        products->vel.length(productsLength);
        products->width.length(productsLength);
        products->snr.length(productsLength);
        products->ncp.length(productsLength);
        
        // Copy the data.
         for (unsigned int i = 0; i < productsLength; i++) {
             int p = 3*i;
            // temprary hack: Sum all four channel powers to create
            // dbz.
            products->dbm[i] = pulses[0]->abp[p+2];
            for (unsigned int f = 1; f < 4; f++)
                products->dbm[i] += pulses[f]->abp[p+2];
            
            // extract the individual channel powers.
            products->p1[i] = pulses[0]->abp[p+2];
            products->p2[i] = pulses[1]->abp[p+2];
            products->p3[i] = pulses[2]->abp[p+2];
            products->p4[i] = pulses[3]->abp[p+2];
            
            products->dbz[i] = 100.0*i;
            products->vel[i] = i/100.0;
            products->width[i] = 100.0*i*i;
            products->ncp[i] = 1.0/(i+1*i+1);
            products->snr[i] = 100*(i-productsLength/2);
        }
        // Publish the products.
        _productsWriter.publishItem(products);
    } else {
        // Oh no, we couldn't get a free products item, so
        // we have to ignore this pulse.
        _droppedPulses++;
    }
    return;
}
////////////////////////////////////////////////////

int EldoraProducts::numPulses() {
    int n = _pulses;
    _pulses = 0;
    return n;
}
////////////////////////////////////////////////////

////////////////////////////////////////////////////

