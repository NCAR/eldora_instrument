#ifndef ELDORAPRODUCTS_INC
#define ELDORAPRODUCTS_INC

#include "PulseTypeSupportC.h"

class EldoraProducts {
    public:
        EldoraProducts();
        ~EldoraProducts();

        void newABPdata(
                EldoraDDS::Pulse* pABP);

    protected:

};
#endif
