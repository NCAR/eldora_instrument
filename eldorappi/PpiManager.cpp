#include "PpiManager.h"

#include <iostream>
/////////////////////////////////////////////////////////////////////////////
PPIManager::PPIManager(
        PPI& ppi, int nProducts, std::vector<ColorMap*>& colorMaps) :
    _ppi(ppi), _nProducts(nProducts),
    _colorMaps(colorMaps), _gates(0){

}
        
/////////////////////////////////////////////////////////////////////////////
PPIManager::~PPIManager() {

}

/////////////////////////////////////////////////////////////////////////////
void PPIManager::newProduct(
        std::vector<double> p, float elDegrees, int prodIndex) {
    
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
    if (_currentProducts.size() == _nProducts) {
        _ppi.addBeam(elDegrees-0.25, elDegrees + 0.25, _gates, _productData, 1, _colorMaps);
        // clear our collected products
        _currentProducts.clear();
    }
}

//////////////////////////////////////////////////////////////////////
void PPIManager::configurePPI(int numProducts, int gates, int beams) {

    _gates = gates;
    
    // allocate the product collection area
    _productData.resize(numProducts);
    for (unsigned int i = 0; i < _productData.size(); i++) 
        _productData[i].resize(_gates);
    
    // configure the display
    _ppi.configure(numProducts, _gates, beams, 0.100*2*_gates, 1);
    
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

