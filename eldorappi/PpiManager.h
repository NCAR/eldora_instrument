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
        /// configure the manager. This cannot take place in the constructor,
        /// becasue the ppi pointer (from the form) will not be initialized
        /// until after setupUI()
        /// @param ppi The ppi display
        /// @param nProducts The number of products that we will be collecting
        /// @param colorMaps The color maps for each product
        void setup(PPI* ppi, int nProducts, std::vector<ColorMap*>* colorMaps);
        /// Add a new data product. The data for product with index
        /// prodIndex are saved in the corresponding vector in _productData.
        /// When a complete set of products have been
        /// received, i.e. when _currentProducts.size() == _nProducts,
        /// _productData are sent to _ppi.
        /// @param p Data for a single product
        /// @param elDegrees The elevation pointing angle
        /// @param prodIndex DEnotes which product in the display this belongs to.
        void newProduct(std::vector<double> p, float elDegrees, int prodIndex);
        /// Configure the PPI displays when there is a change in the operating
        /// configuration, such as the number of gates, etc.
        /// @param numProducts The number of products that the ppi will provide
        /// @param gates The number of gates in each beam
        /// @param beams The number of beams in the ppi
        void configurePPI(int numProducts, int gates, int beams);
        
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
};

#endif /*PPIMANAGER_H_*/
