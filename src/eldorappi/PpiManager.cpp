#include "PpiManager.h"

#include <iostream>
/////////////////////////////////////////////////////////////////////////////
PPIManager::PPIManager() :
    _gates(0), _decimation(1) {
}
/////////////////////////////////////////////////////////////////////////////
void PPIManager::setup(
        PPI* ppi, int nProducts, std::vector<ColorMap*>* colorMaps, int decimation) {

    _ppi = ppi;
    _nProducts = nProducts;
    _colorMaps = colorMaps;
    _decimation = decimation;
}

/////////////////////////////////////////////////////////////////////////////
PPIManager::~PPIManager() {

}

/////////////////////////////////////////////////////////////////////////////
bool PPIManager::newProduct(
        std::vector<double>& p, float rotAngle, int prodIndex) {

    bool retval = false;
    
    // See if we already have this product. If so, then we have dropped
    // one or more products, and need to start over.
    if (_currentProducts.find(prodIndex) != _currentProducts.end()) {
        _currentProducts.clear();
    }

    // record the fact that we have this product
    _currentProducts.insert(prodIndex);

    // save the product data
    _productData[prodIndex] = p;

    // if we have a complete set, send them to the display
    if (_currentProducts.size() == (unsigned int)_nProducts) {
        double startAng = rotAngle-0.45;
        double stopAng = rotAngle+0.45;
        if (startAng < 0.0) {
            startAng += 360.0;
            stopAng += 360.0;
        } else {
            if (stopAng >= 360.0) {
                startAng -= 360.0;
                stopAng -= 360.0;
            }
        }
        _ppi->addBeam(startAng, stopAng, _gates, _productData,
                1, *_colorMaps);
        // clear our collected products
        _currentProducts.clear();
        // indicate that we completed a set
        retval = true;
    }
    return retval;
}

//////////////////////////////////////////////////////////////////////
void PPIManager::configurePPI(
        int numProducts, int gates, int beams) {

    _gates = gates;

    // allocate the product collection area
    _productData.resize(numProducts);
    for (unsigned int i = 0; i < _productData.size(); i++)
        _productData[i].resize(_gates);

    // configure the display
    double distance = 0.100*2*_gates;
    if (gates == 0)
        distance = 100.0;

    _ppi->configure(numProducts, _gates, beams, distance, _decimation);

}
/////////////////////////////////////////////////////////////////////////////
void PPIManager::selectVar(int index) {
    _ppi->selectVar(index);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

