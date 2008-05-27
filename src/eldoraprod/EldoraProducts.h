#ifndef ELDORAPRODUCTS_INC
#define ELDORAPRODUCTS_INC
#include <string>
#include "RayTypeSupportC.h"
#include "ProductsTypeSupportC.h"
#include "DDSPublisher.h"
#include "DDSWriter.h"

class EldoraProducts {
    public:
        EldoraProducts(DDSPublisher& publisher, std::string productsTopic, bool dualPrt);
        virtual ~EldoraProducts();

        void newRayData(
                std::vector<std::vector<EldoraDDS::Ray*> >& pRay);
        
        int numRays();

    protected:
        /// Initialize the fixed fields in a Products item.
        /// @param p te products item to be initialized.
        void initProducts(EldoraDDS::Products* p);
        /// The number of rays that have been received.
        int _rays;
        /// The DDS publisher
        DDSPublisher& _publisher;
        /// The topic name that the Products will be published under.
        std::string _productsTopic;
        /// The DDS Products writer.
        ProductsWriter _productsWriter;
        /// The number of rays which were not processed due to unavailable
        /// empty Products.
        int _droppedRays;
        /// True if we are in dual prt mode
        bool _dualPrt;

};
#endif
