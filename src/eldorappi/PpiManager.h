#ifndef PPIMANAGER_H_
#define PPIMANAGER_H_

#include <set>

#include "ProductTypes.h"
#include "PPI.h"
#include "ColorMap.h"

/// Manage the Eldora data interaction with a PPI display.
class PPIManager {
    public:
        /// Constructor
        PPIManager();
        /// Destructor
        virtual ~PPIManager();
        /// Configure the manager. This cannot take place in the constructor,
        /// becasue the ppi pointer (from the form) will not be initialized
        /// until after setupUI()
        /// @param ppi The ppi display
        /// @param nProducts The number of products that we will be collecting
        /// @param colorMaps The color maps for each product
        /// @param decimation The gate decimation for the PPI
        /// @param minHeight The PPI minimum height in pixels. The width will be set
        /// in order to maintain the clipping planes aspect ratio.
        void setup(PPI* ppi, int nProducts, std::vector<ColorMap*>* colorMaps, 
                   int decimation, int minHeight);
        /// Add a new data product. The data for product with index
        /// prodIndex are saved in the corresponding vector in _productData.
        /// When a complete set of products have been
        /// received, i.e. when _currentProducts.size() == _nProducts,
        /// _productData are sent to _ppi.
        /// @param p Data for a single product
        /// @param rotAngle The radar rotation angle in cartesian coordinates
        /// @param prodIndex DEnotes which product in the display this belongs to.
        /// @returns True if a complete set was assembled on this call.
        bool newProduct(std::vector<double>& p, float rotAngle, int prodIndex);
        /// Configure the PPI displays when there is a change in the operating
        /// configuration, such as the number of gates, etc.
        /// @param numProducts The number of products that the ppi will provide
        /// @param gates The number of gates in each beam
        /// @param beams The number of beams in the ppi
        /// @param gateSizeMeters The gate spacing in meters
        /// @param X coordinate of the left clipping plane (+/- 1.0)
        /// @param X coordinate of the right clipping plane (+/- 1.0)
        /// @param Y coordinate of the bottom clipping plane (+/- 1.0)
        /// @param Y coordinate of the top clipping plane (+/- 1.0)
        void configurePPI(int numProducts, int gates, int beams, double gateSizeMeters=150.0,
                          double left=-1.0, double right=1.0, 
                          double bottom=-1.0, double top=1.0);
        /// Select the product to display on the ppi
        /// @param The product index, zero based.
        void selectVar(int index);
    protected:
        /// The PPI component that we are managing.
        PPI* _ppi;
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
        /// The gate decimation for PPI
        int _decimation;
        /// The minimum ppi height
        int _minHeight;
};

#endif /*PPIMANAGER_H_*/
