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
        ~EldoraProducts();

        void newABPdata(
                EldoraDDS::Pulse* pABP);
        
        int numPulses();

    protected:
        int _pulses;
        DDSPublisher& _publisher;
        std::string _productsTopic;
        ProductsWriter _productsWriter;

};
#endif
