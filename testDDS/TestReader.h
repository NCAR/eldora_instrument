#ifndef TESTREADER_H_
#define TESTREADER_H_

#include "DDSReader.h"
#include "PulseTypeSupportC.h"
#include "PulseTypeSupportImpl.h"
using namespace EldoraDDS;

class TestReader :
	public DDSReader<Pulse, PulseTypeSupportImpl, PulseTypeSupport_var, PulseDataReader, PulseDataReader_var> {
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
#endif /*TESTREADER_H_*/
