#ifndef ELDORASCOPEREADER_H_
#define ELDORASCOPEREADER_H_

#include <vector>
#include <QObject>
#include <QTimer>
#include "DDSReader.h"
#include "PulseTypeSupportC.h"
#include "PulseTypeSupportImpl.h"
#include "TimeSeriesTypeSupportC.h"
#include "TimeSeriesTypeSupportImpl.h"
using namespace EldoraDDS;

/// A subscriber for Eldora data which can feed Qt applications.
/// It is derived from both an EldoraDDS::DDSReader and QObject.
/// When the data notification is received via the notify()
/// method, a Qt signal is emitted in order to deliver the
/// data to other Qt components. Note that this scheme is utilizing 
/// Qt4's capabilty to send signals between threads. See the 
/// Qt4 docuentation on threading and QObjects.
/// @todo Add a throttling mechanism: Use a timer to analyze the
/// incoming samle rate, and from this calcuate a decimation rate 
/// and use this to send the signals with data at a preferred rate.
class EldoraScopeReader : public QObject , public TSReader{
    Q_OBJECT
    
    /// The display can either show all gates along a beam, or
    /// values in time for a selected gate.
    enum GATE_MODE {
        ALONG_BEAM, ///< Display all gates along a beam
        ONE_GATE    ///< Display values in time for a selected gate
    };

    public:
        EldoraScopeReader(
                DDSSubscriber& subscriber,
                    std::string topicName,
                    double outputRate=20.0);
        virtual ~EldoraScopeReader();
        /// Subclass DDSReader::notify(), which wil be called
        /// whenever new samples are added to the DDSReader available
        /// queue. Process the samples here.
        virtual void notify();
        /// Return the number of bytes received. The counter
        /// is reset to zero whenever numBytes() is called.
        unsigned long numBytes();
        
  signals:
       void newData(std::vector<double> I, 
           std::vector<double> Q, 
           double sampleRateHz,
           double tuningFreqHz);
       
   public slots:
   /// Called when the rate timer times out.
       void rateTimeoutSlot();
       
       /// Set the gate mode to ONE_GATE.
       /// @param gate The gate to send in
       /// @param n The number of points
       void oneGateSlot(int channel, int gate, int n);
       
       /// Set the gate mode to along beam.
       void alongBeamSlot(int channel);

    protected:
        /// The number of samples received so far.
        unsigned int _readSamples;
        
        /// The number of data bytes received.
        unsigned long _numBytes;
        
        /// Timer to use for managing output rate
        QTimer _rateTimer;
        
        /// timer interval, ms
        int _intervalMS;
        
        /// The current mode of data delivery.
        GATE_MODE _gateMode;
        
        /// The number of points when in ONE_GATE mode
        int _pointsPerGate;
        
        /// The next data point to be filled in the I and Q vectors.
        int _pointCounter;
        
        /// The gate of choice in ONE_GATE mode.
        int _gate;
        
        /// Set true by the rate timer timeout, which
        /// enables capturing the next block of data
        int _capture;
        
        /// the rate at which to deliver data blocks to the client
        double _outputRate;
        
        /// The selected channel to send to the client
        int _channel;
                
        /// Buffer for saving one gate of I data over successive pulses
        std::vector<double> I;
        
        /// Buffer for saving one gate of Q data over successive pulses
        std::vector<double> Q;
        

};

#endif 
