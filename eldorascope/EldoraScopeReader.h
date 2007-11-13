#ifndef ELDORASCOPEREADER_H_
#define ELDORASCOPEREADER_H_

#include "SdrScope.h"
#include "DDSReader.h"
#include "PulseTypeSupportC.h"
#include "PulseTypeSupportImpl.h"
using namespace EldoraDDS;

/// A templatized class derived from DDSreader based
/// classes such as PulseReader and TSReader. This
/// class simply collects and reports statistics 
/// about the received sample stream from the given
/// topic.
template <class T>
class EldoraScopeReader :
    public T {
public:
    EldoraScopeReader(DDSSubscriber& subscriber, std::string topicName, SdrScope& scope);
    virtual ~EldoraScopeReader();
    /// Subclass DDSReader::notify(), which wil be called
    /// whenever new samples are added to the DDSReader available
    /// queue. Process the samples here.
    virtual void notify();
    /// Return the number of bytes received. The counter
    /// is reset to zero whenever numBytes() is called.
    unsigned long numBytes();

protected:
    SdrScope& _scope;
    unsigned int _readSamples;
    unsigned long _numBytes;

};

#endif 
