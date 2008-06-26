#include "DDSPublisher.h"

#include <ace/streams.h>
#include "ace/Get_Opt.h"

//OpenDDS::DCPS::TransportIdType transport_impl_id = 1;

////////////////////////////////////////////////////////////

DDSPublisher::DDSPublisher(
        int argc,
            char* argv[]) :
    transport_impl_id(1) {
            
    _runStatus = run(argc, argv);
}

////////////////////////////////////////////////////////////

DDSPublisher::~DDSPublisher() {

    // Cleanup

    std::cout << __FUNCTION__ << " destructor\n";

    _participant->delete_contained_entities();

    _dpf->delete_participant(_participant.in());

    TheTransportFactory->release();

    TheServiceParticipant->shutdown();

}

////////////////////////////////////////////////////////////

int DDSPublisher::status() {
    return _runStatus;
}

///////////////////////////////////////////////////////////

int DDSPublisher::run(
        int argc,
            char *argv[]) {
    try
    {
        _dpf = TheParticipantFactoryWithArgs(argc, argv);
        if (!_dpf) {
            cerr << __FILE__ <<":" << __FUNCTION__ << " TheParticipantFactoryWithArgs() failed\n";
            return 1;
        }

        _participant = _dpf->create_participant(411,
                PARTICIPANT_QOS_DEFAULT,
                DDS::DomainParticipantListener::_nil());
        if (CORBA::is_nil (_participant.in ())) {
            cerr << "create_participant failed." << endl;
            return 1;
        }

        // Initialize the transport. Use the existing one if possible.
        OpenDDS::DCPS::TransportImpl_rch tcp_impl;
        try {
            tcp_impl = TheTransportFactory->obtain(transport_impl_id);
	    if (0 == tcp_impl.in ()) {
	      //	cerr << "TheTransportFactory->obtain failed - trying create." << endl;
		tcp_impl =
		TheTransportFactory->create_transport_impl (transport_impl_id,
			::OpenDDS::DCPS::AUTO_CONFIG);
		if (0 == tcp_impl.in () ) {
		    cerr << "TheTransportFactory->create_transport_impl failed." << endl;
		    return 1;
		}
	    }
        } catch (...) {
            tcp_impl =
            TheTransportFactory->create_transport_impl (transport_impl_id,
                    ::OpenDDS::DCPS::AUTO_CONFIG);
	    if (0 == tcp_impl.in () ) {
		cerr << "TheTransportFactory->create_transport_impl failed." << endl;
		return 1;
	    }
        }

        _publisher = _participant->create_publisher(PUBLISHER_QOS_DEFAULT,
                DDS::PublisherListener::_nil());
        if (CORBA::is_nil (_publisher.in ())) {
            cerr << "create_publisher failed." << endl;
            exit(1);
        }

        // Attach the publisher to the transport. (compatible with DDS1.1 API)
        OpenDDS::DCPS::PublisherImpl* pub_impl =
	  dynamic_cast<OpenDDS::DCPS::PublisherImpl*> (_publisher.in ());
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

