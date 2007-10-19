// -*- C++ -*-
// ============================================================================
/**
 *  @file   subscriber.cpp
 *
 *  $Id: subscriber.cpp 899 2007-07-05 16:36:52Z mitza $
 *
 *
 */
// ============================================================================


#include "EldoraSubscriber.h"
#include "EldoraReaderListener.h"
#include "PulseTypeSupportImpl.h"
#include "TimeSeriesTypeSupportImpl.h"
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

#include <ace/streams.h>
#include "ace/Get_Opt.h"

#include <iomanip>
#include <ios>
#include "EldoraDdsC.h"
#include <map>

using namespace EldoraDDS;

OpenDDS::DCPS::TransportIdType transport_impl_id = 1;

////////////////////////////////////////////////////////////

EldoraSubscriber::EldoraSubscriber(std::string topicName) :
	_topicName(topicName) {

}

////////////////////////////////////////////////////////////

EldoraSubscriber::~EldoraSubscriber() {

	if (!CORBA::is_nil(_participant.in())) {
		_participant->delete_contained_entities();
	}
	if (!CORBA::is_nil(_dpf.in())) {
		_dpf->delete_participant(_participant.in());
	}
	ACE_OS::sleep(2);

	TheTransportFactory->release();
	TheServiceParticipant->shutdown();

}

////////////////////////////////////////////////////////////

int EldoraSubscriber::parse_args(int argc, char *argv[]) {
	ACE_Get_Opt get_opts(argc, argv, "t:");
	int c;

	while ((c = get_opts()) != -1) {
		switch (c) {
		case 't':
			if (ACE_OS::strcmp(get_opts.opt_arg(), "udp") == 0) {
				transport_impl_id = 2;
			} else if (ACE_OS::strcmp(get_opts.opt_arg(), "mcast") == 0) {
				transport_impl_id = 3;
			} else if (ACE_OS::strcmp(get_opts.opt_arg(), "reliable_mcast")
					== 0) {
				transport_impl_id = 4;
			}
			// test with DEFAULT_SIMPLE_TCP_ID.
			else if (ACE_OS::strcmp(get_opts.opt_arg(), "default_tcp") == 0) {
				transport_impl_id = OpenDDS::DCPS::DEFAULT_SIMPLE_TCP_ID;
			}
			// test with DEFAULT_SIMPLE_UDP_ID.
			else if (ACE_OS::strcmp(get_opts.opt_arg(), "default_udp") == 0) {
				transport_impl_id = OpenDDS::DCPS::DEFAULT_SIMPLE_UDP_ID;
			} else if (ACE_OS::strcmp(get_opts.opt_arg(), "default_mcast_sub")
					== 0) {
				transport_impl_id = OpenDDS::DCPS::DEFAULT_SIMPLE_MCAST_SUB_ID;
			}
			break;
		case '?':
		default:
			ACE_ERROR_RETURN((LM_ERROR, "usage:  %s "
				"-t <tcp/udp/default> "
				"-u <sleep usecs>"
				"\n", argv [0]), -1);
		}
	}
	// Indicates sucessful parsing of the command line
	return 0;
}

////////////////////////////////////////////////////////////

int EldoraSubscriber::run(int argc, char *argv[]) {
	try
	{
		_dpf = TheParticipantFactoryWithArgs(argc, argv);
		_participant = _dpf->create_participant(411,
				PARTICIPANT_QOS_DEFAULT,
				DDS::DomainParticipantListener::_nil());
		if (CORBA::is_nil (_participant.in ())) {
			cerr << "create_participant failed." << endl;
			return 1;
		}

		if (parse_args (argc, argv) == -1) {
			return -1;
		}

		PulseTypeSupport_var pulsemts = 0;
		TimeSeriesTypeSupport_var tsmts = 0;

		// use this hardwired hack to switch between EldoraPulses
		// and EldoraTimeseries. This will all be replaced by
		// a generic DDSReader soon enough.
		if (!_topicName.compare("EldoraPulses"))
			pulsemts = new PulseTypeSupportImpl();
		else
			tsmts = new TimeSeriesTypeSupportImpl();

		CORBA::String_var type_name;

		if (pulsemts) {
			if (DDS::RETCODE_OK != pulsemts->register_type(_participant.in (), "")) {
				cerr << "Failed to register the MessageTypeTypeSupport." << endl;
				exit(1);
			}
			type_name = pulsemts->get_type_name ();
		} else {
			if (DDS::RETCODE_OK != tsmts->register_type(_participant.in (), "")) {
				cerr << "Failed to register the MessageTypeTypeSupport." << endl;
				exit(1);
			}
			type_name = tsmts->get_type_name ();
		}

		DDS::TopicQos topic_qos;
		_participant->get_default_topic_qos(topic_qos);

		std::cout << "Creating topic " << _topicName.c_str() 
			<< " for type " << type_name << std::endl;
		DDS::Topic_var topic = _participant->create_topic(_topicName.c_str(),
				type_name.in (),
				topic_qos,
				DDS::TopicListener::_nil());
		if (CORBA::is_nil (topic.in ())) {
			std::cerr << "Failed to create_topic " << _topicName << std::endl;
			exit(1);
		}

		// Initialize the transport
		OpenDDS::DCPS::TransportImpl_rch transport_impl =
		TheTransportFactory->create_transport_impl (transport_impl_id,
				::OpenDDS::DCPS::AUTO_CONFIG);

		// Create the subscriber and attach to the corresponding
		// transport.
		DDS::Subscriber_var sub =
		_participant->create_subscriber(SUBSCRIBER_QOS_DEFAULT,
				DDS::SubscriberListener::_nil());
		if (CORBA::is_nil (sub.in ())) {
			cerr << "Failed to create_subscriber." << endl;
			exit(1);
		}

		// Attach the subscriber to the transport.
		OpenDDS::DCPS::SubscriberImpl* sub_impl =
		OpenDDS::DCPS::reference_to_servant<OpenDDS::DCPS::SubscriberImpl> (sub.in ());
		if (0 == sub_impl) {
			cerr << "Failed to obtain subscriber servant\n" << endl;
			exit(1);
		}

		OpenDDS::DCPS::AttachStatus status = sub_impl->attach_transport(transport_impl.in());
		if (status != OpenDDS::DCPS::ATTACH_OK) {
			std::string status_str;
			switch (status) {
				case OpenDDS::DCPS::ATTACH_BAD_TRANSPORT:
				status_str = "ATTACH_BAD_TRANSPORT";
				break;
				case OpenDDS::DCPS::ATTACH_ERROR:
				status_str = "ATTACH_ERROR";
				break;
				case OpenDDS::DCPS::ATTACH_INCOMPATIBLE_QOS:
				status_str = "ATTACH_INCOMPATIBLE_QOS";
				break;
				default:
				status_str = "Unknown Status";
				break;
			}
			cerr << "Failed to attach to the transport. Status == "
			<< status_str.c_str() << endl;
			exit(1);
		}

		// activate the listener
		DDS::DataReaderListener_var listener =
		::OpenDDS::DCPS::servant_to_reference(&_listenerServant);

		if (CORBA::is_nil (listener.in ())) {
			cerr << "listener is nil." << endl;
			exit(1);
		}

		// Create the Datareaders
		DDS::DataReaderQos dr_qos;
		sub->get_default_datareader_qos (dr_qos);
		DDS::DataReader_var dr = sub->create_datareader(topic.in (),
				dr_qos,
				listener.in ());
		if (CORBA::is_nil (dr.in ())) {
			cerr << "create_datareader failed." << endl;
			exit(1);
		}

	}
	catch (CORBA::Exception& e)
	{
		cerr << "SUB: Exception caught in main ():" << endl << e << endl;
		return 1;
	}

	return 0;
}

////////////////////////////////////////////////////////////

int EldoraSubscriber::numBytes() {

	return _listenerServant.numBytes();

}

////////////////////////////////////////////////////////////

std::map<RadarChoice, long> EldoraSubscriber::sequenceErrors() {

	return _listenerServant.sequenceErrors();

}

////////////////////////////////////////////////////////////

