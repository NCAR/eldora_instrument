#include "DDSSubscriber.h"

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

DDSSubscriber::DDSSubscriber(
        int argc,
            char* argv[]) {

    _runStatus = run(argc, argv);

}

////////////////////////////////////////////////////////////

DDSSubscriber::~DDSSubscriber() {

    std::cout << __FUNCTION__ << " destructor\n";

    _participant->delete_contained_entities();

    _dpf->delete_participant(_participant.in());

    TheTransportFactory->release();

    TheServiceParticipant->shutdown();

}

////////////////////////////////////////////////////////////

int DDSSubscriber::status() {
    return _runStatus;
}

////////////////////////////////////////////////////////////

int DDSSubscriber::run(
        int argc,
            char *argv[]) {
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

        // Initialize the transport. Use the existing one if possible.
        OpenDDS::DCPS::TransportImpl_rch transport_impl;
        try {
            transport_impl = TheTransportFactory->obtain(transport_impl_id);
        } catch (...) {
            transport_impl =
            TheTransportFactory->create_transport_impl (transport_impl_id,
                    ::OpenDDS::DCPS::AUTO_CONFIG);
        }

        // Create the subscriber and attach to the corresponding
        // transport.
        _subscriber =
        _participant->create_subscriber(SUBSCRIBER_QOS_DEFAULT,
                DDS::SubscriberListener::_nil());
        if (CORBA::is_nil (_subscriber.in ())) {
            cerr << "Failed to create_subscriber." << endl;
            exit(1);
        }

        // Attach the subscriber to the transport.
        OpenDDS::DCPS::SubscriberImpl* sub_impl =
        OpenDDS::DCPS::reference_to_servant<OpenDDS::DCPS::SubscriberImpl> (_subscriber.in ());
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

    }
    catch (CORBA::Exception& e)
    {
        cerr << "SUB: Exception caught in main ():" << endl << e << endl;
        return 1;
    }

    return 0;
}

////////////////////////////////////////////////////////////

DDS::DomainParticipant_var& DDSSubscriber::getParticipant() {
    return _participant;
}

////////////////////////////////////////////////////////////

DDS::Subscriber_var& DDSSubscriber::getSubscriber() {
    return _subscriber;
}

