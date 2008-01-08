#ifndef ELDORAPRODUCTS_INC
#define ELDORAPRODUCTS_INC
#include <string>
#include "PulseTypeSupportC.h"
#include "ProductsTypeSupportC.h"
#include "DDSPublisher.h"
#include "DDSWriter.h"

class EldoraProducts {
    public:
        EldoraProducts(DDSPublisher& publisher, std::string productsTopic);
        virtual ~EldoraProducts();

        void newPulseData(
                std::vector<EldoraDDS::Pulse*>& pPulse);
        
        int numPulses();

    protected:
        /// Initialize the fixed fields in a Products item.
        /// @param p te products item to be initialized.
        void initProducts(EldoraDDS::Products* p);
        /// The number of pulses that have been received.
        int _pulses;
        /// The DDS publisher
        DDSPublisher& _publisher;
        /// The topic name that the Products will be published under.
        std::string _productsTopic;
        /// The DDS Products writer.
        ProductsWriter _productsWriter;
        /// The number of pulses which were not processed due to unavailable
        /// empty Products.
        int _droppedPulses;

};
#endif
