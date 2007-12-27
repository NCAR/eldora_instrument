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
void EldoraProducts::newABPdata(
        EldoraDDS::Pulse* pulse) {
    _pulses++;

    Products* products = _productsWriter.getEmptyItem();
    if (products) {

        products->radarId = pulse->radarId;
        products->timestamp = pulse->timestamp;
        products->dbz.length(pulse->abp.length()/3);
        int j = 0;
        for (unsigned int i = 0; i < pulse->abp.length(); i+= 3) {
            products->dbz[j++] = pulse->abp[i];
        }

        _productsWriter.publishItem(products);
    } else {
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

