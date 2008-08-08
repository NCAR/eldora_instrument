#include "CappiManager.h"
#include <QApplication>
#include <iostream>
/////////////////////////////////////////////////////////////////////////////
CappiManager::CappiManager() :
    _gates(0), _decimation(1), _minHeight(300) {
}
/////////////////////////////////////////////////////////////////////////////
void CappiManager::setup(
        CAPPI* cappi, int nProducts, 
        std::vector<ColorMap*>* colorMaps, 
        int decimation,
        int minHeight) {

    _cappi = cappi;
    _nProducts = nProducts;
    _colorMaps = colorMaps;
    _decimation = decimation;
    _minHeight = minHeight;
    _stripDisplay = false;
    _stripWidthKm = 2.2;
}

/////////////////////////////////////////////////////////////////////////////
CappiManager::~CappiManager() {

}

/////////////////////////////////////////////////////////////////////////////
bool CappiManager::newProduct(std::vector<double>& p, 
		double xPos, 
		double yPos,
		float rotAngle, 
		int prodIndex) {

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
        // calculate the start and end angle for the ray to be drawn
        double startAng = rotAngle - 0.72;
        double stopAng = rotAngle + 0.72;
        if (startAng < 0.0) {
            startAng += 360.0;
            stopAng += 360.0;
        } else {
            if (stopAng >= 360.0) {
                startAng -= 360.0;
                stopAng -= 360.0;
            }
        }
        
        if (_stripDisplay) {
        	_cappi->addBeam(xPos, yPos, rotAngle, _gates, _productData, 1, *_colorMaps, _stripWidthKm);
        } else {
            _cappi->addBeam(xPos, yPos, startAng, stopAng, _gates, _productData, 1, *_colorMaps);
        }
        // clear our collected products
        _currentProducts.clear();
        // indicate that we completed a set
        retval = true;
    }
    return retval;
}

//////////////////////////////////////////////////////////////////////
void CappiManager::configureCAPPI(
        int numProducts, int gates,  double gateSizeMeters,
        double left, double right, double bottom, double top,
        double spanKm, bool stripDisplay, double stripWidthKm) {
	
	_stripDisplay = stripDisplay;
	
	_stripWidthKm = stripWidthKm;

    _gates = gates;

    // allocate the product collection area
    _productData.resize(numProducts);
    for (unsigned int i = 0; i < _productData.size(); i++)
        _productData[i].resize(_gates);

    // configure the display
    double distance = gateSizeMeters*2*_gates/1000.0;
    if (gates == 0)
        distance = 100.0;

   _cappi->configure(numProducts, _gates,
                    spanKm, 0.150, _decimation, 
                    left, right, bottom, top);
 
}
/////////////////////////////////////////////////////////////////////////////
void CappiManager::selectVar(int index) {
    _cappi->selectVar(index);
}
/////////////////////////////////////////////////////////////////////////////

