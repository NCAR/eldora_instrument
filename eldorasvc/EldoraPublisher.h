#ifndef ELDORAPUBLISHERINC_
#define ELDORAPUBLISHERINC_

#include "PulseTypeSupportImpl.h"
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

#include "EldoraWriter.h"

typedef EldoraDDS::Pulse pulse_t;

class EldoraPublisher {

 public:

  EldoraPublisher();

  virtual ~EldoraPublisher();

  int run(int argc, char* argv[]);

  bool isFinished();

  void publishPulse(pulse_t* pPulse);

  int pulsesAvailable();

  pulse_t* getEmptyPulse();

 protected:

  int parse_args (int argc, char *argv[]);

  DDS::DomainParticipant_var _participant;

  DDS::DomainParticipantFactory_var _dpf;

  DDS::DataWriter_var dw;

  EldoraWriter* _writer;
};

#endif
