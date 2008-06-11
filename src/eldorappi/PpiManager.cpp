#include "PpiManager.h"
#include <QApplication>
#include <iostream>
/////////////////////////////////////////////////////////////////////////////
PPIManager::PPIManager() :
    _gates(0), _decimation(1), _minHeight(300) {
}
/////////////////////////////////////////////////////////////////////////////
void PPIManager::setup(
        PPI* ppi, int nProducts, 
        std::vector<ColorMap*>* colorMaps, 
        int decimation,
        int minHeight) {

    _ppi = ppi;
    _nProducts = nProducts;
    _colorMaps = colorMaps;
    _decimation = decimation;
    _minHeight = minHeight;
    
    // start off with a square display of the minimum height
    _ppi->setMinimumSize(minHeight, minHeight);
    _ppi->repaint();
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
        // convert rotation angle from radar angle (0 points up, angles increase
        // clockwise) to Cartesian angle (0 points right, angles increase
        // counterclockwise), since that's what PPI wants.
        double cartRotAngle = 90.0 - rotAngle;
        // calculate the start and end angle for the ray to be drawn
        double startAng = cartRotAngle - 0.45;
        double stopAng = cartRotAngle + 0.45;
        if (startAng < 0.0) {
            startAng += 360.0;
            stopAng += 360.0;
        } else {
            if (stopAng >= 360.0) {
                startAng -= 360.0;
                stopAng -= 360.0;
            }
        }
        _ppi->addBeam(startAng, stopAng, _gates, _productData, 1, *_colorMaps);
        // clear our collected products
        _currentProducts.clear();
        // indicate that we completed a set
        retval = true;
    }
    return retval;
}

//////////////////////////////////////////////////////////////////////
void PPIManager::configurePPI(
        int numProducts, int gates, int beams, double gateSizeMeters,
        double left, double right, double bottom, double top) {

    _gates = gates;

    // allocate the product collection area
    _productData.resize(numProducts);
    for (unsigned int i = 0; i < _productData.size(); i++)
        _productData[i].resize(_gates);

    // configure the display
    double distance = gateSizeMeters*2*_gates/1000.0;
    if (gates == 0)
        distance = 100.0;

    _ppi->configure(numProducts, _gates, beams, distance, _decimation, 
                    left, right, bottom, top);
    int h = _minHeight;
    int w = h * (right-left)/(top-bottom);
    _ppi->setMinimumSize(w,h);
    _ppi->repaint();
    

}
/////////////////////////////////////////////////////////////////////////////
void PPIManager::selectVar(int index) {
    _ppi->selectVar(index);
}
/////////////////////////////////////////////////////////////////////////////

