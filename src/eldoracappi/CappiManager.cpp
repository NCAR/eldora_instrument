#include "CappiManager.h"
#include <QApplication>
#include <iostream>

/////////////////////////////////////////////////////////////////////////////
CappiManager::CappiManager() :
	_gates(0), _decimation(1), _minHeight(300) {
}
/////////////////////////////////////////////////////////////////////////////
void CappiManager::setup(CAPPI* cappi, int nProducts,
		std::vector<ColorMap*>* colorMaps, int decimation, int minHeight) {

	_cappi = cappi;
	_nProducts = nProducts;
	_colorMaps = colorMaps;
	_decimation = decimation;
	_minHeight = minHeight;
	_stripDisplay = false;

	// initialize to 2.2km on the equator
	_stripWidthDeg = 2.2/111.2;
}

/////////////////////////////////////////////////////////////////////////////
CappiManager::~CappiManager() {

}

/////////////////////////////////////////////////////////////////////////////
bool CappiManager::newProduct(std::vector<double>& p, ptime timetag, double xPos, double yPos,
		float rotAngle, int prodIndex) {

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
			_cappi->addBeam(timetag, xPos, yPos, rotAngle, _gates, _productData, 1,
					*_colorMaps, _stripWidthDeg);
		} else {
			_cappi->addBeam(timetag, xPos, yPos, startAng, stopAng, _gates,
					_productData, 1, *_colorMaps);
		}
		// clear our collected products
		_currentProducts.clear();
		// indicate that we completed a set
		retval = true;
	}
	return retval;
}

//////////////////////////////////////////////////////////////////////
void CappiManager::configureCAPPI(int numProducts, int gates,
		double gateSizeDeg, double spanDeg, bool stripDisplay,
		double stripWidthDeg, double xorigin, double yorigin, seconds timespan) {

	_stripDisplay = stripDisplay;

	_stripWidthDeg = stripWidthDeg;

	_gates = gates;

	// allocate the product collection area
	_productData.resize(numProducts);
	for (unsigned int i = 0; i < _productData.size(); i++)
		_productData[i].resize(_gates);

	_cappi->configure(numProducts, _gates, spanDeg, gateSizeDeg, xorigin,
			yorigin, timespan);

}
/////////////////////////////////////////////////////////////////////////////
void CappiManager::selectVar(int index) {
	_cappi->selectVar(index);
}
/////////////////////////////////////////////////////////////////////////////

