#ifndef READER_H_
#define READER_H_

#include "DDSReader.h"
#include "EldoraDdsTypeSupportC.h"
#include "EldoraDdsTypeSupportImpl.h"
using namespace EldoraDDS;

/// A templatized class derived from DDSreader based
/// classes such as RayReader and TSReader. This
/// class simply collects and reports statistics
/// about the received sample stream from the given
/// topic.
template <class T>
class TestReader :
	public T {
public:
	TestReader(DDSSubscriber& subscriber, std::string topicName);
	virtual ~TestReader();
	/// Subclass DDSReader::notify(), which wil be called
	/// whenever new samples are added to the DDSReader available
	/// queue. Process the samples here.
	virtual void notify();
	/// Return the number of bytes received. The counter
	/// is reset to zero whenever numBytes() is called.
	unsigned long numBytes();

protected:
	unsigned int _readSamples;
	unsigned long _numBytes;

};
#endif /*READER_H_*/
