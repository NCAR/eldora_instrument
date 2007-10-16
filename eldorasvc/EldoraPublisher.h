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

class EldoraPublisher {

 public:

  EldoraPublisher(int argc, char* argv[]);

  virtual ~EldoraPublisher();
  
  int status();

  DDS::DomainParticipant_var& getParticipant() { return _participant; };
  
  DDS::Publisher_var& getPublisher() { return _publisher;} ;


 protected:

  int run(int argc, char* argv[]);

  int parse_args (int argc, char *argv[]);

  DDS::DomainParticipantFactory_var _dpf;

  DDS::Publisher_var _publisher;

  DDS::DomainParticipant_var _participant;
  
  int _runStatus;
};

#endif
