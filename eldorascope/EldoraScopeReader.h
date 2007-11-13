#ifndef ELDORASCOPEREADER_H_
#define ELDORASCOPEREADER_H_

#include <QObject>
#include "DDSReader.h"
#include "PulseTypeSupportC.h"
#include "PulseTypeSupportImpl.h"
using namespace EldoraDDS;

/// A templatized class derived from DDSreader based
/// classes such as PulseReader and TSReader. This
/// class simply collects and reports statistics 
/// about the received sample stream from the given
/// topic.
class EldoraScopeReader : public QObject , public PulseReader{
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

    protected:
        unsigned int _readSamples;
        unsigned long _numBytes;

};

#endif 
