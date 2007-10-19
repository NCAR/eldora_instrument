#ifndef DDSREADERINC_
#define DDSREADERINC_

#include <dds/DdsDcpsSubscriptionS.h>
#include <ace/Task.h>
#include <ace/Synch.h>

#include "DDSSubscriber.h"

/// Template signature for class definitions.
#define TEMPSIG1 class DDSTYPE, class DDSTYPESUPPORTIMPL, class DDSTYPESUPPORT_VAR, class DDSDATAREADER, class DDSDATAREADER_VAR
/// Template signatures for class instantiations.
#define TEMPSIG2 DDSTYPE, DDSTYPESUPPORTIMPL, DDSTYPESUPPORT_VAR, DDSDATAREADER, DDSDATAREADER_VAR

/// A generic reader class.  However, it is really derived from a DataReaderListener.
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

private:

	DDS::DataReader_var _genericReader;
	
	DDSDATAREADER_VAR _specificReader;
	
	/// A queue of subscribed items, waiting to be consumed.
	std::vector<DDSTYPE*> _inQueue;

	/// A queue of empty items, available to receive new data.
	std::vector<DDSTYPE*> _outQueue;
	
	/// Count of samples received. Zeroed after each call to numSamples();
	unsigned int _numSamples;
};

#endif /* DATAREADER_LISTENER_IMPL  */
