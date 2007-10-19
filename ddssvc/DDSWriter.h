#ifndef ELDORAWRITER_H
#define ELDORAWRITER_H

#include <ace/OS_NS_unistd.h>
#include <ace/streams.h>
#include <unistd.h>
#include <dds/DdsDcpsPublicationC.h>
#include <ace/Task.h>
#include <ace/Synch.h>
#include <vector>

#include "DDSPublisher.h"
#include "PulseTypeSupportC.h"
#include "PulseTypeSupportImpl.h"
#include "TimeSeriesTypeSupportC.h"
#include "TimeSeriesTypeSupportImpl.h"

typedef ACE_Thread_Mutex mutex_t;
typedef ACE_Condition_Thread_Mutex condition_t;
typedef ACE_Guard<mutex_t> guard_t;

#define TEMPSIG1 class DDSTYPE, class DDSTYPESUPPORTIMPL, class DDSTYPESUPPORT_VAR, class DDSDATAWRITER, class DDSDATAWRITER_VAR
#define TEMPSIG2 DDSTYPE, DDSTYPESUPPORTIMPL, DDSTYPESUPPORT_VAR, DDSDATAWRITER, DDSDATAWRITER_VAR

/// A writer for an arbitrary DDS datatype. A DDS
/// publisher must be provided; it will perform 
/// the data transfer on behalf of this writer.
/// The template parameters define the datatype which
/// will be written. Currently, five DDS class tye names 
/// must be provided as the template parameters. 
/// DDSWriter provides a queue of buffers. Clients
/// request an empty buffer via getEmptyItem(),
/// fill their data into the item, and return it to the 
/// writer to be publish. Clients can query the number of empty
/// items that are avaialable. Alternatively, they can be
/// suspended in getNextItem() until an empty item becomes
/// available. This bolcking on the queue of empty items
/// is implemented using the ACE condition_t condition variables.
/// @todo Find a way to build all of the required class names
/// from a user supplied root. Simple CPP macros won't work here.
template <TEMPSIG1>
class DDSWriter : public ACE_Task_Base {

public:
	/// Constructor
	/// @param publisher The publisher that this writer will use.
	/// @param topicName The name of the topic that this writer will
	/// publish to.
	DDSWriter(DDSPublisher& publisher, std::string topicName);

	void end();

	/// The service that interacts with the publisher, feeding
	/// it items as they are provided by cleints in other threads.
	/// svc() responds to requests for our topic. When the request 
	/// has been received, then svc() will loop, removing items from
	/// _inQueue, sending them to the publisher, and placing the
	/// empty items back on the _outQueue. If no items are available
	/// on the _inQueue, the loop will block on a condition variable
	/// while waiting for new entried to appear in the _inQueue.
	virtual int svc();

	bool is_finished() const;

	int get_timeout_writes() const;
	/// Put the item on the _inQueue, where it will
	/// be consumed for publishing. Ownership
	/// of pItem is transferred to DDSWriter.
	void publishItem(DDSTYPE* pItem);
	// 2return The number of empty items available
	/// on the _outQueue.
	int itemsAvailable();
	/// Get the next empty item from the _outQueue. If the
	/// _outQueue is empty, block the calling process until 
	/// an item becomes available.
	DDSTYPE* getEmptyItem();

private:
	/// block on _condition until the _inQueue is not empty.
	void waitForItem();
	/// Take the next item on the _inQueue, send it to the 
	/// _publisher, and then return the item to the _outQueue.
	bool publish(::DDS::InstanceHandle_t handle);

	/// The initial writer, not specialized for a data type
	DDS::DataWriter_var _genericWriter;

	/// The data writer specialized for our specific data type.
	DDSDATAWRITER_VAR _specificWriter;

	ACE_Atomic_Op<ACE_SYNCH_MUTEX, int> finished_instances_;

	ACE_Atomic_Op<ACE_SYNCH_MUTEX, int> timeout_writes_;

	mutex_t _mutex;

	condition_t _condition;

	/// A queue of incoming items, waiting to be published.
	std::vector<DDSTYPE*> _inQueue;

	/// A queue of empty items, available for clients to use.
	std::vector<DDSTYPE*> _outQueue;

};

#endif
