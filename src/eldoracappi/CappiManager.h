#ifndef CAPPPIMANAGER_H_
#define CAPPPIMANAGER_H_

#include <set>

#include "CAPPI.h"
#include "ColorMap.h"

/// Manage the Eldora data interaction with a CAPPI display.
class CappiManager {
public:
	/// Constructor
	CappiManager();
	/// Destructor
	virtual ~CappiManager();
	/// Configure the manager. This cannot take place in the constructor,
	/// becasue the cappi pointer (from the form) will not be initialized
	/// until after setupUI()
	/// @param cappi The cappi display
	/// @param nProducts The number of products that we will be collecting
	/// @param colorMaps The color maps for each product
	/// @param decimation The gate decimation for the PPI
	/// @param minHeight The PPI minimum height in pixels. The width will be set
	/// in order to maintain the clipping planes aspect ratio.
	void setup(CAPPI* cappi, int nProducts, std::vector<ColorMap*>* colorMaps,
			int decimation, int minHeight);
	/// Add a new data product. The data for product with index
	/// prodIndex are saved in the corresponding vector in _productData.
	/// When a complete set of products have been
	/// received, i.e. when _currentProducts.size() == _nProducts,
	/// _productData are sent to _ppi.
	/// @param p Data for a single product
	/// @param timetag The timetag for this beam.
	/// @param rotAngle The radar rotation angle in cartesian coordinates
	/// @param prodIndex DEnotes which product in the display this belongs to.
	/// @returns True if a complete set was assembled on this call.
	bool newProduct(std::vector<double>& p, double timetag, double xPos, double yPos,
			float rotAngle, int prodIndex);
	/// Configure the CAPPI displays when there is a change in the operating
	/// configuration, such as the number of gates, etc.
	/// @param numProducts The number of products that the ppi will provide
	/// @param gates The number of gates in each beam
	/// @param beams The number of beams in the ppi
	/// @param gateSizeDeg The gate spacing in degrees
	/// @param spanDeg The X and Y span across the display, in degrees.
	/// @param stripDisplay True if the beams are to be displayed as strips
	/// @param stripWidthDeg The width of the strips in degrees.
	/// @param timespan The time span for culling beams.
	void configureCAPPI(int numProducts, int gates, double gateSizeDeg,
			double spanDeg, bool stripDisplay, double stripWidthDeg,
			double xorigin, double yorigin, double timespan);
	/// Select the product to display on the ppi
	/// @param The product index, zero based.
	void selectVar(int index);
protected:
	/// The CAPPI component that we are managing.
	CAPPI* _cappi;
	/// The number of products to be collected
	int _nProducts;
	/// This set contains the list of all received products 
	/// that are on the desired list, and have the same beam id.
	/// There is one set for the forward radar and one set for the aft.
	/// complete set of products has been received, a new beam can 
	/// be drawn.
	std::set<int> _currentProducts;
	/// Will hold the beam values for all product variables in one beam,
	/// for a given radar.
	std::vector<std::vector<double> > _productData;
	/// Reference to the colormaps for the products, one for each product
	std::vector<ColorMap*>* _colorMaps;
	/// The current number of gates
	int _gates;
	/// The gate decimation for CAPPI
	int _decimation;
	/// The minimum ppi height
	int _minHeight;
	/// true if beams are to be displayed as strips.
	bool _stripDisplay;
	/// the width of beam strips
	double _stripWidthDeg;
};

#endif /*CAPPIMANAGER_H_*/
