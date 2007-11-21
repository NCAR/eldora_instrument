
#ifndef RR314SIMINC_
#define RR314SIMINC_

#include "RR314.h"
#include <pthread.h>

namespace RedRapids {

  /// A data simulator for the RR314. This
  /// class can be used by the RR314 class as 
  /// a simulated data feed. It creates a thread
  /// which will periodically call RR314::newData()
  /// with synthetic data. By using a thread to 
  /// deliver the bogus data stream, this conveniently
  /// mimics the asynchronous thread like behavior
  /// of the data delivery from a real RR314 interrupt.
  class RR314sim 
  {

  public:
    /// Constructor. 
    /// @param pRR314 The RR314 device that we want to provide 
    /// simulated data for.
    /// @param gates The number of gates
    /// @param startiq The starting gate for iq data.
    /// @param numiq The number of iq gates.
    /// @param nci The number of IQs that go into each ABP
    RR314sim(RedRapids::RR314* pRR314, 
	     int gates, 
	     int startiq, 
	     int numiq, 
	     int nci);
    /// Destructor.
    virtual ~RR314sim();
    /// Start the simulation thread. 
    void start();

  protected:
    /// The function that is called to start the simulation thread
    static void* simThread(void* threadArgs);
    /// Perform the actual simulation. An infinite
    /// loop periodically calls _pRR314->newData()
    void simulate();
    /// The RR314 object that receives the simulation data.
    RedRapids::RR314* _pRR314;
    /// The dmaChan numbers which receive IQ data
    std::vector<int> _iqChans;
    /// The dmaChan numbers which receive ABP data.
    std::vector<int> _abpChans;
    /// A buffer used as the source of ABP data.
    std::vector<int> _abp;
    /// A buffer used as the source of IQ data.
    std::vector<short> _iq;
    /// The number of gates
    int _gates;
    /// The starting iq gate
    int _startiq;
    /// The number of iqgates
    int _numiq;
    /// The number of coherent integrations
    int _nci;
    /// The pulse number
    unsigned int _pulseNum;
    /// The beam number
    unsigned int _beamNum;
};
}
#endif
