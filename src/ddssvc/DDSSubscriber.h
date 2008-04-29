#ifndef DDSSUBCSRIBERINC_
#define DDSSUBSCRIBERINC_

#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/SubscriberImpl.h>
#include <dds/DCPS/transport/framework/TheTransportFactory.h>
#include <dds/DCPS/transport/simpleTCP/SimpleTcpConfiguration.h>
#ifdef ACE_AS_STATIC_LIBS
#include <dds/DCPS/transport/simpleTCP/SimpleTcp.h>
#include <dds/DCPS/transport/simpleUnreliableDgram/SimpleUnreliableDgram.h>
#include <dds/DCPS/transport/ReliableMulticast/ReliableMulticast.h>
#endif

#include <string>
/// A generic DDS subscriber. It is paired with DDSReader 
/// to subscribe to a specific topic. Multiple DDSReaders can
/// use the same DDSSubscriber to access multiple topics.
/// DDSSubscriber creates the domain participant, the subscriber,
/// and the transport.
/// @todo Encapsulate functions which require access to 
/// the participant and the subscriber. Currently, references
/// to the participant   and the subscriber are made available
/// and are used by DDSReaader for some registration functions.
/// These really should not be visible outside of the class.
class DDSSubscriber {

public:

	/// Constructor. Command line standard OpenDDS arguments 
	/// are passed to the constructor.
	/// @param argc The number of arguments.
	/// @param argv The arguments
	DDSSubscriber(int argc, char* argv[]);

	virtual ~DDSSubscriber();

	int status();

	/// @return The participant. It is required for
	/// type registration and creation of topics
	DDS::DomainParticipant_var& getParticipant();

	/// @return The subscriber. It is required because the
	/// subscriber creates readers.
	DDS::Subscriber_var& getSubscriber();

protected:

	int run(int argc, char* argv[]);

	DDS::DomainParticipant_var _participant;

	DDS::DomainParticipantFactory_var _dpf;
	
	DDS::Subscriber_var _subscriber;

	int _runStatus;

};

#endif
