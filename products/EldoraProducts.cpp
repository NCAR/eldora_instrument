#include "EldoraProducts.h"
#include <iostream>
////////////////////////////////////////////////////
EldoraProducts::EldoraProducts(
        DDSPublisher& publisher, std::string productsTopic) :
    _pulses(0), _publisher(publisher), _productsTopic(productsTopic),
    _productsWriter(publisher, productsTopic){

}

////////////////////////////////////////////////////
EldoraProducts::~EldoraProducts() {

}

////////////////////////////////////////////////////
void EldoraProducts::newABPdata(
        EldoraDDS::Pulse* p) {
    _pulses++;
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

