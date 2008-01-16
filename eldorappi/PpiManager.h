#ifndef PPIMANAGER_H_
#define PPIMANAGER_H_

#include <set>

#include "ProductTypes.h"


#include "PPI.h"

/// Manage the Eldora data interaction with a PPI display.
class PPIManager {
    public:
        /// Constructor
        /// @param ppi The ppi display
        /// @param nProducts The number of products that we will be collecting
        PPIManager(PPI& ppi, int nProducts);
        /// Destructor
        virtual ~PPIManager();
        /// Add a new data product. The data for product with index
        /// prodIndex are saved in the corresponding vector in _productData.
        /// When a complete set of products have been
        /// received, i.e. when _currentProducts.size() == _nProducts,
        /// _productData are sent to _ppi.
        /// @param p Data for a single product
        /// @param elDegrees The elevation pointing angle
        /// @param prodIndex DEnotes which product in the display this belongs to.
        void newProduct(std::vector<double> p, float elDegrees, int prodIndex);
        
    protected:
        /// The PPI component that we are managing.
        PPI& _ppi;
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
        
};

#endif /*PPIMANAGER_H_*/
