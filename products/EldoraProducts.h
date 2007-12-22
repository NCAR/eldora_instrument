#ifndef ELDORAPRODUCTS_INC
#define ELDORAPRODUCTS_INC

#include "PulseTypeSupportC.h"

class EldoraProducts {
    public:
        EldoraProducts();
        ~EldoraProducts();

        void newABPdata(
                EldoraDDS::Pulse* pABP);
        
        int numPulses();

    protected:
        int _pulses;

};
#endif
