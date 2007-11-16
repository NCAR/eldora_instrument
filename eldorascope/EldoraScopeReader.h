#ifndef ELDORASCOPEREADER_H_
#define ELDORASCOPEREADER_H_

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
    public:
        EldoraScopeReader(
                DDSSubscriber& subscriber,
                    std::string topicName);
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
       void rateTimeoutSlot();

    protected:
        /// The number of samples received so far.
        unsigned int _readSamples;
        
        /// The number of data bytes received.
        unsigned long _numBytes;
        
        /// Output rate, samples per second.
        int _outputRate;
        
        /// sample downcounter, used to establish
        /// the output rate. It is initialzed to
        /// _rate/_outputRate
        int _downCounter;
                
        /// Timer to use for sample rate measurement
        QTimer _rateTimer;
        
        /// timer interval, ms
        int _intervalMS;
        
        /// The currently calculated rate, hz
        double _rate;
        
        /// The last sample count when the rate timer signalled
        unsigned long _lastSampleCount;

};

#endif 
