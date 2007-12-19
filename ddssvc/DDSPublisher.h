#ifndef ELDORAPUBLISHERINC_
#define ELDORAPUBLISHERINC_

#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/PublisherImpl.h>
#include <dds/DCPS/transport/framework/TheTransportFactory.h>
#include <dds/DCPS/transport/simpleTCP/SimpleTcpConfiguration.h>
#ifdef ACE_AS_STATIC_LIBS
#include <dds/DCPS/transport/simpleTCP/SimpleTcp.h>
#include <dds/DCPS/transport/simpleUnreliableDgram/SimpleUnreliableDgram.h>
#include <dds/DCPS/transport/ReliableMulticast/ReliableMulticast.h>
#endif

#include <ace/Synch.h>

/// Encapsulate a DDS Publisher. One DDSPublisher can be used by 
/// multiple DDSwriter s. The publisher runs in a separate thread,
/// which is internally created in the run() method.
class DDSPublisher {

    public:

        /// Constructor
        /// @param argc The number of arguments
        /// @param argv Arguments confirming to -DCPS* options (see the OCI
        /// "TAO developer's Guide Excerpt, section 31.11)
        DDSPublisher(
                int argc,
                    char* argv[]);

        /// Destructor
        virtual ~DDSPublisher();

        /// Get the publisher run status. Currently will
        /// be 0 if the publisher was created succesfully, non-zero otherwise.
        /// @return The status.
        int status();

        /// Get the a reference to the  DDS participant.
        /// @return A reference to the domain participant. This is needed by
        /// writers. 
        /// @todo Add api so that the writers can call DDSPublisher for 
        /// the required participant functionality, rather than exposing the participant.
        DDS::DomainParticipant_var& getParticipant() {
            return _participant;
        }
        ;

        /// Get the a reference to the  DDS publisher.
        /// @return A reference to the DDS publisher. This is needed by
        /// writers. 
        /// @todo Add api so that the writers can call DDSPublisher for 
        /// the required publisher  functionality, rather than exposing the publisher.
        DDS::Publisher_var& getPublisher() {
            return _publisher;
        }
        ;

    protected:

        /// Run the publisher threads(s).
        /// @param argc The number of arguments
        /// @param argv Arguments confirming to -DCPS* options (see the OCI
        /// "TAO developer's Guide Excerpt, section 31.11)
        int run(
                int argc,
                    char* argv[]);

        /// The domain participant factory
        DDS::DomainParticipantFactory_var _dpf;

        /// The DDS publisher
        DDS::Publisher_var _publisher;

        /// The domain participant
        DDS::DomainParticipant_var _participant;

        /// The transport type id
        OpenDDS::DCPS::TransportIdType transport_impl_id;

        /// The status returned from trying to run the publisher.
        /// 0 if okay, non-zero otherwise
        int _runStatus;
};

#endif
