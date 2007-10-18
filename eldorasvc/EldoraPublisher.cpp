#include "DDSPublisher.h"

#include <ace/streams.h>
#include "ace/Get_Opt.h"

OpenDDS::DCPS::TransportIdType transport_impl_id = 1;

////////////////////////////////////////////////////////////

DDSPublisher::DDSPublisher(int argc, char* argv[]) {

	_runStatus = run(argc, argv);
}

////////////////////////////////////////////////////////////

DDSPublisher::~DDSPublisher() {

	// Cleanup
	_participant->delete_contained_entities();

	_dpf->delete_participant(_participant.in());

	TheTransportFactory->release();

	TheServiceParticipant->shutdown();

}

////////////////////////////////////////////////////////////

int DDSPublisher::status() {
	return _runStatus;
}

////////////////////////////////////////////////////////////

int DDSPublisher::parse_args(int argc, char *argv[]) {
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

int DDSPublisher::run(int argc, char *argv[]) {
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

		OpenDDS::DCPS::TransportImpl_rch tcp_impl =
		TheTransportFactory->create_transport_impl (transport_impl_id,
				::OpenDDS::DCPS::AUTO_CONFIG);

		_publisher = _participant->create_publisher(PUBLISHER_QOS_DEFAULT,
				DDS::PublisherListener::_nil());
		if (CORBA::is_nil (_publisher.in ())) {
			cerr << "create_publisher failed." << endl;
			exit(1);
		}

		// Attach the publisher to the transport.
		OpenDDS::DCPS::PublisherImpl* pub_impl =
		OpenDDS::DCPS::reference_to_servant<OpenDDS::DCPS::PublisherImpl> (_publisher.in ());
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

