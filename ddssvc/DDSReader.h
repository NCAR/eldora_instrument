#ifndef DDSREADERINC_
#define DDSREADERINC_

#include <dds/DdsDcpsSubscriptionS.h>
#include <ace/Task.h>
#include <ace/Synch.h>
typedef ACE_Thread_Mutex mutex_t;
typedef ACE_Condition_Thread_Mutex condition_t;
typedef ACE_Guard<mutex_t> guard_t;

#include "DDSSubscriber.h"


/// Template signature for class definitions.
#undef TEMPSIG1  // avoid "TEMPSIG1 redefined" warnings
#define TEMPSIG1 class DDSTYPE, class DDSTYPESUPPORTIMPL, class DDSTYPESUPPORT_VAR, class DDSDATAREADER, class DDSDATAREADER_VAR
/// Template signatures for class instantiations.
#undef TEMPSIG2  // avoid "TEMPSIG2 redefined" warnings
#define TEMPSIG2 DDSTYPE, DDSTYPESUPPORTIMPL, DDSTYPESUPPORT_VAR, DDSDATAREADER, DDSDATAREADER_VAR

/// A generic data reader class which is templated on the DDS data type. Instantiate
/// this class with the desired data type to create a reader for a particular
/// topic. It requires and works with an existing DDSSubscriber. 
///
/// Newly arrived data are placed on a queue. They are removed from the
/// queue with getNextItem(). The item must be returned when no longer 
/// needed with returnItem().
///
/// New data can be detected either by polling or by a notification scheme.
/// For polling, use itemsAvailable() to ask how many items are available, 
/// or just verify that getNextItem() returns a non-null. To use the notification
/// scheme, derive from this class and subclass the notify() method. It
/// will be called whenever new data arrives and is placed on the queue,
/// which can tehn be retrieved with getNextItem().
/// 
/// This class is actually derived from a DDS::DataReaderListener, so that it
/// can receive the asynchronous notifications via the on_data_available()
/// function. The class creates and contains a DDS:DataReader for accessing
/// the delivered data.
template<TEMPSIG1>
class DDSReader: public virtual OpenDDS::DCPS::LocalObject<DDS::DataReaderListener> 
{
	
public:
	//Constructor
	/// Constructor
	/// @param subscriber The subscriber that this reader will use.
	/// @param topicName The name of the topic that this reader will
	/// subscribe to.
	DDSReader(DDSSubscriber& subscriber, std::string topicName);

	//Destructor
	virtual ~DDSReader(void);

	// @return The number of full items available
	/// on the _outQueue.
	int itemsAvailable();
	
	/// Get the next full item from the _outQueue. If
	/// nothing is avaiable, a null is returned.
	DDSTYPE* getNextItem();
	
	/// Return an item to be placed back on the _inQueue.
	/// @param pItem The item to be returned.
	void returnItem(DDSTYPE* pItem);
	
	/// This function will be called after each on_data_available()
	/// notification, when samples have been added to the queue.
	/// Subclass DDSReader and implement notify() in order to
	/// respond to new data. Call getNextItem() to fetch new data.
	virtual void notify();

	virtual void on_requested_deadline_missed(DDS::DataReader_ptr reader,
			const DDS::RequestedDeadlineMissedStatus & status)
			throw (CORBA::SystemException);

	virtual void on_requested_incompatible_qos(DDS::DataReader_ptr reader,
			const DDS::RequestedIncompatibleQosStatus & status)
			throw (CORBA::SystemException);

	virtual void on_liveliness_changed(DDS::DataReader_ptr reader,
			const DDS::LivelinessChangedStatus & status)
			throw (CORBA::SystemException);

	virtual void on_subscription_match(DDS::DataReader_ptr reader,
			const DDS::SubscriptionMatchStatus & status)
			throw (CORBA::SystemException);

	virtual void on_sample_rejected(DDS::DataReader_ptr reader,
			const DDS::SampleRejectedStatus& status)
			throw (CORBA::SystemException);

	virtual void on_data_available(DDS::DataReader_ptr reader)
			throw (CORBA::SystemException);

	virtual void on_sample_lost(DDS::DataReader_ptr reader,
			const DDS::SampleLostStatus& status) throw (CORBA::SystemException);
	
	/// @return The total number of samples received since the
	/// last call to numSamples()
	unsigned int numSamples();

	/// @return The total number of samples dropped since the
	/// last call to droppedSamples()
	unsigned int droppedSamples();

private:

	DDS::DataReader_var _genericReader;
	
	DDSDATAREADER_VAR _specificReader;
	
	/// A queue of subscribed items, waiting to be consumed.
	std::vector<DDSTYPE*> _inQueue;

	/// A queue of empty items, available to receive new data.
	std::vector<DDSTYPE*> _outQueue;
	
	/// A mutex that protects acces to the queues
	mutex_t _queueMutex;
	
	/// Count of samples received. Zeroed after each call to numSamples();
	unsigned int _numSamples;
	
	/// number of dropped items. Zeroed when numDropped is called
	unsigned int _droppedSamples;
	
	/// The topic name
	std::string _topicName;
	
};

#include "PulseTypeSupportC.h"
#include "PulseTypeSupportImpl.h"
#include "TimeSeriesTypeSupportC.h"
#include "TimeSeriesTypeSupportImpl.h"
#include "ProductsTypeSupportC.h"
#include "ProductsTypeSupportImpl.h"

using namespace EldoraDDS;
typedef DDSReader<Pulse, PulseTypeSupportImpl, PulseTypeSupport_var, PulseDataReader, PulseDataReader_var> PulseReader;
typedef DDSReader<TimeSeries, TimeSeriesTypeSupportImpl, TimeSeriesTypeSupport_var, TimeSeriesDataReader, TimeSeriesDataReader_var> TSReader;
typedef DDSReader<Products, ProductsTypeSupportImpl, ProductsTypeSupport_var, ProductsDataReader, ProductsDataReader_var> ProductsReader;

#endif /* DATAREADER_LISTENER_IMPL  */
