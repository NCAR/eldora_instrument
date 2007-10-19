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

class DDSSubscriber {

public:

	DDSSubscriber(int argc, char* argv[]);

	virtual ~DDSSubscriber();

	int status();

	DDS::DomainParticipant_var& getParticipant();

	DDS::Subscriber_var& getSubscriber();

protected:

	int run(int argc, char* argv[]);

	int parse_args(int argc, char *argv[]);

	DDS::DomainParticipant_var _participant;

	DDS::DomainParticipantFactory_var _dpf;
	
	DDS::Subscriber_var _subscriber;

	int _runStatus;

};

#endif
