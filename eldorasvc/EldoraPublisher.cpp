#include "EldoraPublisher.h"

#include <ace/streams.h>
#include "ace/Get_Opt.h"

OpenDDS::DCPS::TransportIdType transport_impl_id = 1;

////////////////////////////////////////////////////////////

EldoraPublisher::EldoraPublisher() :
	_writer(0) {
}

////////////////////////////////////////////////////////////

EldoraPublisher::~EldoraPublisher() {

	// Cleanup
	_writer->end();

	delete _writer;

	_participant->delete_contained_entities();

	_dpf->delete_participant(_participant.in());

	TheTransportFactory->release();

	TheServiceParticipant->shutdown();

}

////////////////////////////////////////////////////////////

int EldoraPublisher::parse_args(int argc, char *argv[]) {
	ACE_Get_Opt get_opts(argc, argv, "t:u:");
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
			} else if (ACE_OS::strcmp(get_opts.opt_arg(), "default_mcast_pub")
					== 0) {
				transport_impl_id = OpenDDS::DCPS::DEFAULT_SIMPLE_MCAST_PUB_ID;
			}
			break;
		case '?':
		default:
			ACE_ERROR_RETURN((LM_ERROR, "usage:  %s "
				"-t <tcp/udp/default> "
				"\n", argv [0]), -1);
		}
	}
	// Indicates sucessful parsing of the command line
	return 0;
}

////////////////////////////////////////////////////////////

int EldoraPublisher::run(int argc, char *argv[]) {
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

		EldoraDDS::PulseTypeSupport_var mts = new EldoraDDS::PulseTypeSupportImpl();

		if (DDS::RETCODE_OK != mts->register_type(_participant.in (), "")) {
			cerr << "register_type failed." << endl;
			exit(1);
		}

		CORBA::String_var type_name = mts->get_type_name ();

		DDS::TopicQos topic_qos;
		_participant->get_default_topic_qos(topic_qos);
		DDS::Topic_var topic =
		_participant->create_topic ("Movie Discussion List",
				type_name.in (),
				topic_qos,
				DDS::TopicListener::_nil());
		if (CORBA::is_nil (topic.in ())) {
			cerr << "create_topic failed." << endl;
			exit(1);
		}

		OpenDDS::DCPS::TransportImpl_rch tcp_impl =
		TheTransportFactory->create_transport_impl (transport_impl_id,
				::OpenDDS::DCPS::AUTO_CONFIG);

		DDS::Publisher_var pub =
		_participant->create_publisher(PUBLISHER_QOS_DEFAULT,
				DDS::PublisherListener::_nil());
		if (CORBA::is_nil (pub.in ())) {
			cerr << "create_publisher failed." << endl;
			exit(1);
		}

		// Attach the publisher to the transport.
		OpenDDS::DCPS::PublisherImpl* pub_impl =
		OpenDDS::DCPS::reference_to_servant<OpenDDS::DCPS::PublisherImpl> (pub.in ());
		if (0 == pub_impl) {
			cerr << "Failed to obtain publisher servant" << endl;
			exit(1);
		}

		OpenDDS::DCPS::AttachStatus status = pub_impl->attach_transport(tcp_impl.in());
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

		// Create the datawriter
		DDS::DataWriterQos dw_qos;
		pub->get_default_datawriter_qos (dw_qos);
		dw = pub->create_datawriter(topic.in (),
				dw_qos,
				DDS::DataWriterListener::_nil());
		if (CORBA::is_nil (dw.in ())) {
			cerr << "create_datawriter failed." << endl;
			exit(1);
		}

		_writer = new EldoraWriter(dw.in());

		_writer->start ();

	}
	catch (CORBA::Exception& e)
	{
		cerr << "PUB: Exception caught in main.cpp:" << endl
		<< e << endl;
		exit(1);
	}

	return 0;
}

////////////////////////////////////////////////////////////

bool EldoraPublisher::isFinished() {
	return _writer->is_finished();
}

////////////////////////////////////////////////////////////

int EldoraPublisher::pulsesAvailable() {

	return _writer->pulsesAvailable();

}

////////////////////////////////////////////////////////////

pulse_t* EldoraPublisher::getEmptyPulse() {

	return _writer->getEmptyPulse();

}

////////////////////////////////////////////////////////////

void EldoraPublisher::publishPulse(pulse_t* pPulse) {
	
	std::cout << __FILE__ << " received a pulse\n";

	_writer->publishPulse(pPulse);

}

